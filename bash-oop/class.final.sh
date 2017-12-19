#!/bin/bash -e

DEFCLASS_2BA4876520844E069F7B429BB77F5ADE=""
CLASS_2BA4876520844E069F7B429BB77F5ADE=""
THIS_2BA4876520844E069F7B429BB77F5ADE=0
 
class() {
  DEFCLASS_2BA4876520844E069F7B429BB77F5ADE="$1"
  eval CLASS_${DEFCLASS_2BA4876520844E069F7B429BB77F5ADE}_VARS=""
  eval CLASS_${DEFCLASS_2BA4876520844E069F7B429BB77F5ADE}_FUNCTIONS=""
}
 
func() {
  local varname="CLASS_${DEFCLASS_2BA4876520844E069F7B429BB77F5ADE}_FUNCTIONS"
  eval "$varname=\"\${$varname}$1 \""
}
 
var() {
  local varname="CLASS_${DEFCLASS_2BA4876520844E069F7B429BB77F5ADE}_VARS"
  eval $varname="\"\${$varname}$1 \""
}

# backup global variables with duplicate names
saveglobal() {
  eval "varlist=\"\$CLASS_${CLASS_2BA4876520844E069F7B429BB77F5ADE}_VARS\""
  local repo="saved_values_2BA4876520844E069F7B429BB77F5ADE"
  local i
  for var in $varlist; do
    ((++i))
    eval "$repo[$i]=\$${var}"
  done
}
 
loadvar() {
  eval "varlist=\"\$CLASS_${CLASS_2BA4876520844E069F7B429BB77F5ADE}_VARS\""
  for var in $varlist; do
    eval "$var=\"\$INSTANCE_${THIS_2BA4876520844E069F7B429BB77F5ADE}_$var\""
  done
}

# make it possible to invoke methods internally
loadfunc() {
  eval "funclist=\"\$CLASS_${CLASS_2BA4876520844E069F7B429BB77F5ADE}_FUNCTIONS\""
  for func in $funclist; do
    eval "${func}() { ${CLASS_2BA4876520844E069F7B429BB77F5ADE}::${func} \"\$@\"; return \$?; }"
  done
}

unloadfunc() {
  eval "funclist=\"\$CLASS_${CLASS_2BA4876520844E069F7B429BB77F5ADE}_FUNCTIONS\""
  for func in $funclist; do unset ${func}; done
}

# restore global variables
restoreglobal() {
  eval "varlist=\"\$CLASS_${CLASS_2BA4876520844E069F7B429BB77F5ADE}_VARS\""
  local repo="saved_values_2BA4876520844E069F7B429BB77F5ADE"
  local temp i
  for var in $varlist; do
    ((++i))
    eval "temp=\$\{$repo\[\$i\]\}"
    eval "${var}=${temp}"
  done
}
 
savevar() {
  eval "varlist=\"\$CLASS_${CLASS_2BA4876520844E069F7B429BB77F5ADE}_VARS\""
  for var in $varlist; do
    eval "INSTANCE_${THIS_2BA4876520844E069F7B429BB77F5ADE}_$var=\"\$$var\""
  done
}
 
typeof() {
  local objectName=$1
  eval id=\$$objectName
  eval echo \$\{TYPEOF_$id\}
}

# new <class name> <object name> <parameters for constructor>
new() {
  local class="$1"
  local cvar="$2"
  shift
  shift
  local id=$(uuidgen | sed -e 's/-//g' -e 's/.*/\L&/')
  eval TYPEOF_${id}=$class
  eval $cvar=$id
  local funclist
  eval "funclist=\"\$CLASS_${class}_FUNCTIONS\""
  for func in $funclist; do
    local load="saveglobal; loadvar; loadfunc"
    # avoid loading var for constructor because they are unbound
    if [ "$func" = "$class" ]; then load="saveglobal; loadfunc"; fi
    eval "${cvar}.${func}() {
      local t=\$THIS_2BA4876520844E069F7B429BB77F5ADE; THIS_2BA4876520844E069F7B429BB77F5ADE=$id; local c=\$CLASS_2BA4876520844E069F7B429BB77F5ADE; CLASS_2BA4876520844E069F7B429BB77F5ADE=$class; $load; 
      ${class}::${func} \"\$@\"; rt=\$?; savevar; restoreglobal; unloadfunc; CLASS_2BA4876520844E069F7B429BB77F5ADE=\$c; THIS_2BA4876520844E069F7B429BB77F5ADE=\$t; return \$rt;
    }"
  done
  eval "${cvar}.${class} \"\$@\" || true"
}
