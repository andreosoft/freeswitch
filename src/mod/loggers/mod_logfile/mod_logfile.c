/*
 * FreeSWITCH Modular Media Switching Software Library / Soft-Switch Application
 *
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is FreeSWITCH Modular Media Switching Software Library / Soft-Switch Application
 *
 * The Initial Developer of the Original Code is
 * Anthony Minessale II <anthmct@yahoo.com>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Anthony LaMantia <anthony@petabit.net>
 * Michael Jerris <mike@jerris.com>
 *
 *
 * mod_logfile.c -- Filesystem Logging
 *
 */

#include <switch.h>

SWITCH_MODULE_LOAD_FUNCTION(mod_logfile_load);
SWITCH_MODULE_SHUTDOWN_FUNCTION(mod_logfile_shutdown);
SWITCH_MODULE_DEFINITION(mod_logfile, mod_logfile_load, mod_logfile_shutdown, NULL);

#define DEFAULT_LIMIT	 0xA00000 /* About 10 MB */
#define WARM_FUZZY_OFFSET 256
#define MAX_ROT 4096 /* why not */

static switch_memory_pool_t *module_pool = NULL;
static switch_hash_t *profile_hash = NULL;

static struct {
	int rotate;
	switch_mutex_t *mutex;
} globals;

struct logfile_profile {
	char *name;
	switch_size_t log_size;	  /* keep the log size in check for rotation */
	switch_size_t roll_size;  /* the size that we want to rotate the file at */
	char *logfile;
	switch_file_t *log_afd;
    switch_hash_t *log_hash;
    uint32_t all_level;
};

typedef struct logfile_profile logfile_profile_t;

static switch_status_t load_profile(switch_xml_t xml);

#if 0
static void del_mapping(char *var, logfile_profile_t *profile)
{
	switch_core_hash_insert(profile->log_hash, var, NULL);
}
#endif

static void add_mapping(logfile_profile_t *profile, char *var, char *val)
{
	if (!strcasecmp(var, "all")) {
		profile->all_level |= (uint32_t) switch_log_str2mask(val);
		return;
	}

	switch_core_hash_insert(profile->log_hash, var, (void *)(intptr_t) switch_log_str2mask(val));
}

static switch_status_t mod_logfile_rotate(logfile_profile_t *profile);

static switch_status_t mod_logfile_openlogfile(logfile_profile_t *profile, switch_bool_t check)
{
	unsigned int flags = 0;
	switch_file_t *afd;
	switch_status_t stat;

	flags |= SWITCH_FOPEN_CREATE;
	flags |= SWITCH_FOPEN_READ;
	flags |= SWITCH_FOPEN_WRITE;
	flags |= SWITCH_FOPEN_APPEND;

	stat = switch_file_open(&afd, profile->logfile, flags, SWITCH_FPROT_UREAD|SWITCH_FPROT_UWRITE, module_pool);
	if (stat != SWITCH_STATUS_SUCCESS) {
		return SWITCH_STATUS_FALSE;
	}

	profile->log_afd = afd;

	profile->log_size = switch_file_get_size(profile->log_afd);

	if (check && profile->roll_size && profile->log_size >= profile->roll_size) {
		mod_logfile_rotate(profile);
	}

	return SWITCH_STATUS_SUCCESS;
}

/* rotate the log file */
static switch_status_t mod_logfile_rotate(logfile_profile_t *profile)
{
	unsigned int i = 0;
	char *p = NULL;
	switch_status_t stat = 0;
	int64_t offset = 0;
	switch_memory_pool_t *pool;
	switch_time_exp_t tm;
	char date[80] = "";
	switch_size_t retsize;
	switch_status_t status = SWITCH_STATUS_SUCCESS;

	switch_mutex_lock(globals.mutex);

	switch_time_exp_lt(&tm, switch_timestamp_now());
	switch_strftime(date, &retsize, sizeof(date), "%Y-%m-%d-%H-%M-%S", &tm);

	profile->log_size = 0;

	stat = switch_file_seek(profile->log_afd, SWITCH_SEEK_SET, &offset);

	if (stat != SWITCH_STATUS_SUCCESS) {
		status = SWITCH_STATUS_FALSE;
		goto end;
	}


	p = malloc(strlen(profile->logfile)+WARM_FUZZY_OFFSET);
	switch_assert(p);

	memset(p, '\0', strlen(profile->logfile)+WARM_FUZZY_OFFSET);

	switch_core_new_memory_pool(&pool);

	for (i=1; i < MAX_ROT; i++) {
		sprintf((char *)p, "%s.%s.%i", profile->logfile, date, i);
		if (switch_file_exists(p, pool) == SWITCH_STATUS_SUCCESS) {
			continue;
		}

		switch_file_close(profile->log_afd);
		switch_file_rename(profile->logfile, p, pool);
		if ((status = mod_logfile_openlogfile(profile, SWITCH_FALSE)) != SWITCH_STATUS_SUCCESS) {
            switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_CRIT, "Error Rotating Log!\n");
            goto end;
        }
		break;
	}

	free(p);

	switch_core_destroy_memory_pool(&pool);

	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_NOTICE, "New log started.\n");

end:

	switch_mutex_unlock(globals.mutex);

	return status;
}

/* write to the actual logfile */
static switch_status_t mod_logfile_raw_write(logfile_profile_t *profile, char *log_data)
{
	switch_size_t len;
    switch_status_t status = SWITCH_STATUS_SUCCESS;
	len = strlen(log_data);

	if (len <= 0 || !profile->log_afd) {
		return SWITCH_STATUS_FALSE;
	}

	switch_mutex_lock(globals.mutex);

	if (switch_file_write(profile->log_afd, log_data, &len) != SWITCH_STATUS_SUCCESS) {
		switch_file_close(profile->log_afd);
		if ((status = mod_logfile_openlogfile(profile, SWITCH_TRUE)) == SWITCH_STATUS_SUCCESS) {
            len = strlen(log_data);
            switch_file_write(profile->log_afd, log_data, &len);
        }
	}

	switch_mutex_unlock(globals.mutex);

    if (status == SWITCH_STATUS_SUCCESS) {
        profile->log_size += len;

        if (profile->roll_size && profile->log_size >= profile->roll_size) {
            mod_logfile_rotate(profile);
        }
    }

	return status;
}

static switch_status_t process_node(const switch_log_node_t *node, switch_log_level_t level)
{
    switch_hash_index_t *hi;
	void *val;
	const void *var;
	logfile_profile_t *profile;
    
    for (hi = switch_hash_first(NULL, profile_hash); hi; hi = switch_hash_next(hi)) {
        size_t mask = 0;
        size_t ok = 0;
    
        switch_hash_this(hi, &var, NULL, &val);
        profile = val;
        
        ok = switch_log_check_mask(profile->all_level, level);
        
        if (!ok) {
            mask = (size_t) switch_core_hash_find(profile->log_hash, node->file);
            ok = switch_log_check_mask(mask, level);
        }

        if (!ok) {
            mask = (size_t) switch_core_hash_find(profile->log_hash, node->func);
            ok = switch_log_check_mask(mask, level);
        }

        if (ok) {
            mod_logfile_raw_write(profile, node->data);
        }

    }

	return SWITCH_STATUS_SUCCESS;
}

static switch_status_t mod_logfile_logger(const switch_log_node_t *node, switch_log_level_t level)
{
    return process_node(node, level);
}

static switch_status_t load_profile(switch_xml_t xml)
{
	switch_xml_t param, settings;
    char *name = (char *) switch_xml_attr_soft(xml, "name");
    logfile_profile_t *new_profile;

    new_profile = switch_core_alloc(module_pool, sizeof(*new_profile));
    memset(new_profile, 0, sizeof(*new_profile));
    switch_core_hash_init(&(new_profile->log_hash), module_pool);
    new_profile->name = switch_core_strdup(module_pool, switch_str_nil(name));
    
    
	if ((settings = switch_xml_child(xml, "settings"))) {
        for (param = switch_xml_child(settings, "param"); param; param = param->next) {
            char *var = (char *) switch_xml_attr_soft(param, "name");
            char *val = (char *) switch_xml_attr_soft(param, "value");
            if (!strcmp(var, "logfile")) {
                new_profile->logfile = strdup(val);
            } else if (!strcmp(var, "rollover")) {
                new_profile->roll_size = atoi(val);
                if (new_profile->roll_size < 0) {
                    new_profile->roll_size = 0;
                }
            }
        }
    }

	if ((settings = switch_xml_child(xml, "mappings"))) {
		for (param = switch_xml_child(settings, "map"); param; param = param->next) {
			char *var = (char *) switch_xml_attr_soft(param, "name");
			char *val = (char *) switch_xml_attr_soft(param, "value");
            
			add_mapping(new_profile, var, val);
		}
	}

	if (switch_strlen_zero(new_profile->logfile)) {
		char logfile[512];
		switch_snprintf(logfile, sizeof(logfile), "%s%s%s", SWITCH_GLOBAL_dirs.log_dir, SWITCH_PATH_SEPARATOR, "freeswitch.log");
		new_profile->logfile = strdup(logfile);
	}
    
    if (mod_logfile_openlogfile(new_profile, SWITCH_TRUE) != SWITCH_STATUS_SUCCESS) {
        return SWITCH_STATUS_GENERR;
    }

    switch_core_hash_insert(profile_hash, new_profile->name, (void *) new_profile);
    return SWITCH_STATUS_SUCCESS;
}


static void event_handler(switch_event_t *event)
{
	const char *sig = switch_event_get_header(event, "Trapped-Signal");
    switch_hash_index_t *hi;
	void *val;
	const void *var;
	logfile_profile_t *profile;

	if (sig && !strcmp(sig, "HUP")) {
		if (globals.rotate) {
		    for (hi = switch_hash_first(NULL, profile_hash); hi; hi = switch_hash_next(hi)) {
		        switch_hash_this(hi, &var, NULL, &val);
				profile = val;
				mod_logfile_rotate(profile);
			}
		} else {
			switch_mutex_lock(globals.mutex);
		    for (hi = switch_hash_first(NULL, profile_hash); hi; hi = switch_hash_next(hi)) {
		        switch_hash_this(hi, &var, NULL, &val);
				profile = val;
				switch_file_close(profile->log_afd);
				if (mod_logfile_openlogfile(profile, SWITCH_TRUE) != SWITCH_STATUS_SUCCESS) {
                    switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_CRIT, "Error Re-opening Log!\n");
                }
			}
			switch_mutex_unlock(globals.mutex);
		}
	}
}

SWITCH_MODULE_LOAD_FUNCTION(mod_logfile_load)
{
	char *cf = "logfile.conf";
	switch_xml_t cfg, xml, settings, param, profiles, xprofile;

	module_pool = pool;

	memset(&globals, 0, sizeof(globals));
	switch_mutex_init(&globals.mutex, SWITCH_MUTEX_NESTED, module_pool);

	if (profile_hash) {
		switch_core_hash_destroy(&profile_hash);
	}
	switch_core_hash_init(&profile_hash, module_pool);

	if (switch_event_bind((char *) modname, SWITCH_EVENT_TRAP, SWITCH_EVENT_SUBCLASS_ANY, event_handler, NULL) != SWITCH_STATUS_SUCCESS) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Couldn't bind!\n");
		return SWITCH_STATUS_GENERR;
	}

	/* connect my internal structure to the blank pointer passed to me */
	*module_interface = switch_loadable_module_create_module_interface(pool, modname);

	if (!(xml = switch_xml_open_cfg(cf, &cfg, NULL))) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "open of %s failed\n", cf);
	} else {
		if ((settings = switch_xml_child(cfg, "settings"))) {
			for (param = switch_xml_child(settings, "param"); param; param = param->next) {
				char *var = (char *) switch_xml_attr_soft(param, "name");
				char *val = (char *) switch_xml_attr_soft(param, "value");
				if (!strcmp(var, "rotate-on-hup")) {
					globals.rotate = switch_true(val);
				}
			}
		}

		if ((profiles = switch_xml_child(cfg, "profiles"))) {
			for (xprofile = switch_xml_child(profiles, "profile"); xprofile; xprofile = xprofile->next) {
                if (load_profile(xprofile) != SWITCH_STATUS_SUCCESS) {
                    switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "error loading profile.");
                }
            }
        }

		switch_xml_free(xml);
	}

	switch_log_bind_logger(mod_logfile_logger, SWITCH_LOG_DEBUG);

	return SWITCH_STATUS_SUCCESS;
}

SWITCH_MODULE_SHUTDOWN_FUNCTION(mod_logfile_shutdown)
{
	/* TODO:  Need to finish processing pending log messages before we close the file handle */

	//switch_file_close(globals->log_afd);
	return SWITCH_STATUS_SUCCESS;
}

/* For Emacs:
 * Local Variables:
 * mode:c
 * indent-tabs-mode:t
 * tab-width:4
 * c-basic-offset:4
 * End:
 * For VIM:
 * vim:set softtabstop=4 shiftwidth=4 tabstop=4 expandtab:
 */
