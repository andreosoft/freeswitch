/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.35
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.freeswitch.swig;

public class freeswitch {
  public static void consoleLog(String level_str, String msg) {
    freeswitchJNI.consoleLog(level_str, msg);
  }

  public static void consoleCleanLog(String msg) {
    freeswitchJNI.consoleCleanLog(msg);
  }

  public static void console_log(String level_str, String msg) {
    freeswitchJNI.console_log(level_str, msg);
  }

  public static void console_clean_log(String msg) {
    freeswitchJNI.console_clean_log(msg);
  }

  public static void bridge(CoreSession session_a, CoreSession session_b) {
    freeswitchJNI.bridge(CoreSession.getCPtr(session_a), session_a, CoreSession.getCPtr(session_b), session_b);
  }

  public static SWIGTYPE_p_switch_status_t hanguphook(SWIGTYPE_p_switch_core_session_t session) {
    return new SWIGTYPE_p_switch_status_t(freeswitchJNI.hanguphook(SWIGTYPE_p_switch_core_session_t.getCPtr(session)), true);
  }

  public static SWIGTYPE_p_switch_status_t dtmf_callback(SWIGTYPE_p_switch_core_session_t session, SWIGTYPE_p_void input, SWIGTYPE_p_switch_input_type_t itype, SWIGTYPE_p_void buf, long buflen) {
    return new SWIGTYPE_p_switch_status_t(freeswitchJNI.dtmf_callback(SWIGTYPE_p_switch_core_session_t.getCPtr(session), SWIGTYPE_p_void.getCPtr(input), SWIGTYPE_p_switch_input_type_t.getCPtr(itype), SWIGTYPE_p_void.getCPtr(buf), buflen), true);
  }

  public static void setJavaVM(SWIGTYPE_p_JavaVM value) {
    freeswitchJNI.javaVM_set(SWIGTYPE_p_JavaVM.getCPtr(value));
  }

  public static SWIGTYPE_p_JavaVM getJavaVM() {
    long cPtr = freeswitchJNI.javaVM_get();
    return (cPtr == 0) ? null : new SWIGTYPE_p_JavaVM(cPtr, false);
  }

}
