# This file was automatically generated by SWIG (http://www.swig.org).
# Version 1.3.35
#
# Don't modify this file, modify the SWIG interface instead.

package freeswitch;
require Exporter;
@ISA = qw(Exporter);
package freeswitchc;
boot_freeswitch();
package freeswitch;
@EXPORT = qw( );

# ---------- BASE METHODS -------------

package freeswitch;

sub TIEHASH {
    my ($classname,$obj) = @_;
    return bless $obj, $classname;
}

sub CLEAR { }

sub FIRSTKEY { }

sub NEXTKEY { }

sub FETCH {
    my ($self,$field) = @_;
    my $member_func = "swig_${field}_get";
    $self->$member_func();
}

sub STORE {
    my ($self,$field,$newval) = @_;
    my $member_func = "swig_${field}_set";
    $self->$member_func($newval);
}

sub this {
    my $ptr = shift;
    return tied(%$ptr);
}


# ------- FUNCTION WRAPPERS --------

package freeswitch;

*consoleLog = *freeswitchc::consoleLog;
*consoleCleanLog = *freeswitchc::consoleCleanLog;
*console_log = *freeswitchc::console_log;
*console_clean_log = *freeswitchc::console_clean_log;
*bridge = *freeswitchc::bridge;
*hanguphook = *freeswitchc::hanguphook;
*dtmf_callback = *freeswitchc::dtmf_callback;
*api_execute = *freeswitchc::api_execute;
*api_reply_delete = *freeswitchc::api_reply_delete;

############# Class : freeswitch::API ##############

package freeswitch::API;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( freeswitch );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = freeswitchc::new_API(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        freeswitchc::delete_API($self);
        delete $OWNER{$self};
    }
}

*execute = *freeswitchc::API_execute;
*executeString = *freeswitchc::API_executeString;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : freeswitch::input_callback_state_t ##############

package freeswitch::input_callback_state_t;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( freeswitch );
%OWNER = ();
%ITERATORS = ();
*swig_function_get = *freeswitchc::input_callback_state_t_function_get;
*swig_function_set = *freeswitchc::input_callback_state_t_function_set;
*swig_threadState_get = *freeswitchc::input_callback_state_t_threadState_get;
*swig_threadState_set = *freeswitchc::input_callback_state_t_threadState_set;
*swig_extra_get = *freeswitchc::input_callback_state_t_extra_get;
*swig_extra_set = *freeswitchc::input_callback_state_t_extra_set;
*swig_funcargs_get = *freeswitchc::input_callback_state_t_funcargs_get;
*swig_funcargs_set = *freeswitchc::input_callback_state_t_funcargs_set;
sub new {
    my $pkg = shift;
    my $self = freeswitchc::new_input_callback_state_t(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        freeswitchc::delete_input_callback_state_t($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : freeswitch::Stream ##############

package freeswitch::Stream;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( freeswitch );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = freeswitchc::new_Stream(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        freeswitchc::delete_Stream($self);
        delete $OWNER{$self};
    }
}

*write = *freeswitchc::Stream_write;
*get_data = *freeswitchc::Stream_get_data;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : freeswitch::Event ##############

package freeswitch::Event;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( freeswitch );
%OWNER = ();
%ITERATORS = ();
*swig_event_get = *freeswitchc::Event_event_get;
*swig_event_set = *freeswitchc::Event_event_set;
*swig_serialized_string_get = *freeswitchc::Event_serialized_string_get;
*swig_serialized_string_set = *freeswitchc::Event_serialized_string_set;
*swig_mine_get = *freeswitchc::Event_mine_get;
*swig_mine_set = *freeswitchc::Event_mine_set;
sub new {
    my $pkg = shift;
    my $self = freeswitchc::new_Event(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        freeswitchc::delete_Event($self);
        delete $OWNER{$self};
    }
}

*serialize = *freeswitchc::Event_serialize;
*setPriority = *freeswitchc::Event_setPriority;
*getHeader = *freeswitchc::Event_getHeader;
*getBody = *freeswitchc::Event_getBody;
*getType = *freeswitchc::Event_getType;
*addBody = *freeswitchc::Event_addBody;
*addHeader = *freeswitchc::Event_addHeader;
*delHeader = *freeswitchc::Event_delHeader;
*fire = *freeswitchc::Event_fire;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : freeswitch::CoreSession ##############

package freeswitch::CoreSession;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( freeswitch );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        freeswitchc::delete_CoreSession($self);
        delete $OWNER{$self};
    }
}

*swig_session_get = *freeswitchc::CoreSession_session_get;
*swig_session_set = *freeswitchc::CoreSession_session_set;
*swig_channel_get = *freeswitchc::CoreSession_channel_get;
*swig_channel_set = *freeswitchc::CoreSession_channel_set;
*swig_flags_get = *freeswitchc::CoreSession_flags_get;
*swig_flags_set = *freeswitchc::CoreSession_flags_set;
*swig_allocated_get = *freeswitchc::CoreSession_allocated_get;
*swig_allocated_set = *freeswitchc::CoreSession_allocated_set;
*swig_cb_state_get = *freeswitchc::CoreSession_cb_state_get;
*swig_cb_state_set = *freeswitchc::CoreSession_cb_state_set;
*swig_hook_state_get = *freeswitchc::CoreSession_hook_state_get;
*swig_hook_state_set = *freeswitchc::CoreSession_hook_state_set;
*answer = *freeswitchc::CoreSession_answer;
*preAnswer = *freeswitchc::CoreSession_preAnswer;
*hangup = *freeswitchc::CoreSession_hangup;
*setVariable = *freeswitchc::CoreSession_setVariable;
*setPrivate = *freeswitchc::CoreSession_setPrivate;
*getPrivate = *freeswitchc::CoreSession_getPrivate;
*getVariable = *freeswitchc::CoreSession_getVariable;
*recordFile = *freeswitchc::CoreSession_recordFile;
*setCallerData = *freeswitchc::CoreSession_setCallerData;
*originate = *freeswitchc::CoreSession_originate;
*setDTMFCallback = *freeswitchc::CoreSession_setDTMFCallback;
*speak = *freeswitchc::CoreSession_speak;
*set_tts_parms = *freeswitchc::CoreSession_set_tts_parms;
*collectDigits = *freeswitchc::CoreSession_collectDigits;
*getDigits = *freeswitchc::CoreSession_getDigits;
*transfer = *freeswitchc::CoreSession_transfer;
*playAndGetDigits = *freeswitchc::CoreSession_playAndGetDigits;
*streamFile = *freeswitchc::CoreSession_streamFile;
*flushEvents = *freeswitchc::CoreSession_flushEvents;
*flushDigits = *freeswitchc::CoreSession_flushDigits;
*setAutoHangup = *freeswitchc::CoreSession_setAutoHangup;
*setHangupHook = *freeswitchc::CoreSession_setHangupHook;
*ready = *freeswitchc::CoreSession_ready;
*execute = *freeswitchc::CoreSession_execute;
*sendEvent = *freeswitchc::CoreSession_sendEvent;
*begin_allow_threads = *freeswitchc::CoreSession_begin_allow_threads;
*end_allow_threads = *freeswitchc::CoreSession_end_allow_threads;
*get_uuid = *freeswitchc::CoreSession_get_uuid;
*get_cb_args = *freeswitchc::CoreSession_get_cb_args;
*check_hangup_hook = *freeswitchc::CoreSession_check_hangup_hook;
*run_dtmf_callback = *freeswitchc::CoreSession_run_dtmf_callback;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : freeswitch::Session ##############

package freeswitch::Session;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( freeswitch::CoreSession freeswitch );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = freeswitchc::new_Session(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        freeswitchc::delete_Session($self);
        delete $OWNER{$self};
    }
}

*begin_allow_threads = *freeswitchc::Session_begin_allow_threads;
*end_allow_threads = *freeswitchc::Session_end_allow_threads;
*check_hangup_hook = *freeswitchc::Session_check_hangup_hook;
*run_dtmf_callback = *freeswitchc::Session_run_dtmf_callback;
*swig_session_get = *freeswitchc::Session_session_get;
*swig_session_set = *freeswitchc::Session_session_set;
*swig_channel_get = *freeswitchc::Session_channel_get;
*swig_channel_set = *freeswitchc::Session_channel_set;
*swig_flags_get = *freeswitchc::Session_flags_get;
*swig_flags_set = *freeswitchc::Session_flags_set;
*swig_allocated_get = *freeswitchc::Session_allocated_get;
*swig_allocated_set = *freeswitchc::Session_allocated_set;
*swig_cb_state_get = *freeswitchc::Session_cb_state_get;
*swig_cb_state_set = *freeswitchc::Session_cb_state_set;
*swig_hook_state_get = *freeswitchc::Session_hook_state_get;
*swig_hook_state_set = *freeswitchc::Session_hook_state_set;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


# ------- VARIABLE STUBS --------

package freeswitch;

*S_HUP = *freeswitchc::S_HUP;
*S_FREE = *freeswitchc::S_FREE;
*S_RDLOCK = *freeswitchc::S_RDLOCK;
1;
