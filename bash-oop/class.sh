#!/bin/bash -e

DEFCLASS=""
CLASS=""
THIS=0
 
class() {
  DEFCLASS="$1"
  eval CLASS_${DEFCLASS}_VARS=""
  eval CLASS_${DEFCLASS}_FUNCTIONS=""
}
 
func() {
  local varname="CLASS_${DEFCLASS}_FUNCTIONS"
  eval "$varname=\"\${$varname}$1 \""
}
 
var() {
  local varname="CLASS_${DEFCLASS}_VARS"
  eval $varname="\"\${$varname}$1 \""
}

# backup global variables with duplicate names
saveglobal() {
  eval "varlist=\"\$CLASS_${CLASS}_VARS\""
  local repo="saved_globals"
  local i
  for var in $varlist; do
    ((++i))
    eval "$repo[$i]=\$${var}"
  done
}
 
loadvar() {
  eval "varlist=\"\$CLASS_${CLASS}_VARS\""
  for var in $varlist; do
    eval "$var=\"\$INSTANCE_${THIS}_$var\""
  done
}

# make it possible to invoke methods internally
loadfunc() {
  eval "funclist=\"\$CLASS_${CLASS}_FUNCTIONS\""
  for func in $funclist; do
    eval "${func}() { ${CLASS}::${func} \"\$@\"; return \$?; }"
  done
}

unloadfunc() {
  eval "funclist=\"\$CLASS_${CLASS}_FUNCTIONS\""
  for func in $funclist; do unset ${func}; done
}

# restore global variables
restoreglobal() {
  eval "varlist=\"\$CLASS_${CLASS}_VARS\""
  local repo="saved_globals"
  local temp i
  for var in $varlist; do
    ((++i))
    eval "temp=\$\{$repo\[\$i\]\}"
    eval "${var}=${temp}"
  done
}
 
savevar() {
  eval "varlist=\"\$CLASS_${CLASS}_VARS\""
  for var in $varlist; do
    eval "INSTANCE_${THIS}_$var=\"\$$var\""
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
  local obj="$2"
  shift
  shift
  local id=$(uuidgen | sed -e 's/-//g' -e 's/.*/\L&/')
  eval TYPEOF_${id}=$class
  eval $obj=$id
  local funclist
  eval "funclist=\"\$CLASS_${class}_FUNCTIONS\""
  for func in $funclist; do
    local load="saveglobal; loadvar; loadfunc"
    # avoid loading var for constructor because they are unbound
    if [ "$func" = "$class" ]; then load="saveglobal; loadfunc"; fi
    eval "${obj}.${func}() {
      local t=\$THIS; THIS=$id; local c=\$CLASS; CLASS=$class; $load; 
      ${class}::${func} \"\$@\"; rt=\$?; savevar; restoreglobal; unloadfunc; CLASS=\$c; THIS=\$t; return \$rt;
    }"
  done
  eval "${obj}.${class} \"\$@\" || true"
}
