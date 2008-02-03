/* 
 * FreeSWITCH Modular Media Switching Software Library / Soft-Switch Application
 * Copyright (C) 2005/2006, Anthony Minessale II <anthmct@yahoo.com>
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
 * Michael Jerris <mike@jerris.com>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * 
 * Michael Jerris <mike@jerris.com>
 *
 *
 * switch_regex.c -- PCRE wrapper
 *
 */

#include <switch.h>
#include <pcre.h>

SWITCH_DECLARE(switch_regex_t *) switch_regex_compile(const char *pattern,
														 int options, const char **errorptr, int *erroroffset, 
const unsigned char *tables) 
{
	
return pcre_compile(pattern, options, errorptr, erroroffset, tables);

}

SWITCH_DECLARE(int) switch_regex_copy_substring(const char *subject, int *ovector, int stringcount, int stringnumber, char *buffer, int size)
{
	return pcre_copy_substring(subject, ovector, stringcount, stringnumber, buffer, size);
}

SWITCH_DECLARE(void) switch_regex_free(void *data)
{
	pcre_free(data);

} 

SWITCH_DECLARE(int) switch_regex_perform(const char *field, const char *expression, switch_regex_t **new_re, int *ovector, uint32_t olen)
{
	const char *error = NULL;
	int erroffset = 0;
	pcre *re = NULL;
	int match_count = 0;
	char *tmp = NULL;
	uint32_t flags = 0;

	if (!(field && expression)) {
		return 0;
	}

	if (*expression == '/') {
		char *opts = NULL;
		tmp = strdup(expression + 1);
		assert(tmp);
		if ((opts = strrchr(tmp, '/'))) {
			*opts++ = '\0';
		} else {
			goto end;
		}
		expression = tmp;
		if (opts) {
			if (strchr(opts, 'i')) {
				flags |= PCRE_CASELESS;
			}
			if (strchr(opts, 's')) {
				flags |= PCRE_DOTALL;
			}
		}
	}

	re = pcre_compile(expression,	/* the pattern */
					  flags,		/* default options */
					  &error,	/* for error message */
					  &erroffset,	/* for error offset */
					  NULL);	/* use default character tables */
	if (error) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "COMPILE ERROR: %d [%s][%s]\n", erroffset, error, expression);
		switch_regex_safe_free(re);
		goto end;
	}

	match_count = pcre_exec(re,	/* result of pcre_compile() */
							NULL,	/* we didn't study the pattern */
							field,	/* the subject string */
							(int) strlen(field),	/* the length of the subject string */
							0,	/* start at offset 0 in the subject */
							0,	/* default options */
							ovector,	/* vector of integers for substring information */
							olen);	/* number of elements (NOT size in bytes) */


	if (match_count <= 0) {
		switch_regex_safe_free(re);
		match_count = 0;
	}

	*new_re = (switch_regex_t *) re;

 end:
	switch_safe_free(tmp);
	return match_count;
}

SWITCH_DECLARE(void) switch_perform_substitution(switch_regex_t *re, int match_count, const char *data, const char *field_data,
												 char *substituted, switch_size_t len, int *ovector)
{
	char index[10] = "";
	char replace[1024] = "";
	switch_size_t x, y = 0, z = 0;
	int num = 0;

	for (x = 0; x < (len - 1) && x < strlen(data);) {
		if (data[x] == '$') {
			x++;

			if (!(data[x] > 47 && data[x] < 58)) {
				substituted[y++] = data[x - 1];
				continue;
			}

			while (data[x] > 47 && data[x] < 58) {
				index[z++] = data[x];
				x++;
			}
			index[z++] = '\0';
			z = 0;
			num = atoi(index);

			if (pcre_copy_substring(field_data, ovector, match_count, num, replace, sizeof(replace)) > 0) {
				switch_size_t r;
				for (r = 0; r < strlen(replace); r++) {
					substituted[y++] = replace[r];
				}
			}
		} else {
			substituted[y++] = data[x];
			x++;
		}
	}
	substituted[y++] = '\0';
}

SWITCH_DECLARE(switch_status_t) switch_regex_match(const char *target, const char *expression)
{
	const char *error = NULL;	//Used to hold any errors
	int error_offset = 0;		//Holds the offset of an error
	pcre *pcre_prepared = NULL;	//Holds the compiled regex
	int match_count = 0;		//Number of times the regex was matched
	int offset_vectors[2];		//not used, but has to exist or pcre won't even try to find a match

	//Compile the expression
	pcre_prepared = pcre_compile(expression, 0, &error, &error_offset, NULL);

	//See if there was an error in the expression
	if (error != NULL) {
		//Clean up after ourselves
		if (pcre_prepared) {
			pcre_free(pcre_prepared);
			pcre_prepared = NULL;
		}
		//Note our error    
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR,
						  "Regular Expression Error expression[%s] error[%s] location[%d]\n", expression, error, error_offset);

		//We definitely didn't match anything
		return SWITCH_STATUS_FALSE;
	}
	//So far so good, run the regex
	match_count = pcre_exec(pcre_prepared, NULL, target, (int) strlen(target), 0, 0, offset_vectors, sizeof(offset_vectors) / sizeof(offset_vectors[0]));

	//Clean up
	if (pcre_prepared) {
		pcre_free(pcre_prepared);
		pcre_prepared = NULL;
	}

	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "number of matches: %d\n", match_count);

	//Was it a match made in heaven?
	if (match_count > 0) {
		return SWITCH_STATUS_SUCCESS;
	} else {
		return SWITCH_STATUS_FALSE;
	}
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
