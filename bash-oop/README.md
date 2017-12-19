### Implementation Bash of encapsulation feature in OOP
- class.sh is the implementation
- class.test.sh test the class code
- class.final.sh reduce the namespace pollution to minimal and should be used in serious scenarios. Please use the following code to generate the final version

```
magic="2BA4876520844E069F7B429BB77F5ADE"

# convert readable version to final version
sed -e 's/\bCLASS_${DEFCLASS}_VARS\b/CLASS_${DEFCLASS}_VARS_2BA4876520844E069F7B429BB77F5ADE/g' -e 's/\bCLASS_${DEFCLASS}_FUNCTIONS\b/CLASS_${DEFCLASS}_FUNCTIONS_2BA4876520844E069F7B429BB77F5ADE/g' -e 's/CLASS_${CLASS}_FUNCTIONS/CLASS_${CLASS}_FUNCTIONS_2BA4876520844E069F7B429BB77F5ADE/g' -e 's/CLASS_${CLASS}_VARS/CLASS_${CLASS}_VARS_2BA4876520844E069F7B429BB77F5ADE/g' -e 's/CLASS_${class}_FUNCTIONS/CLASS_${class}_FUNCTIONS_2BA4876520844E069F7B429BB77F5ADE/g' class.sh > class.final.sh

sed -e "s/\bCLASS\b/CLASS_$magic/g" -e "s/\bDEFCLASS\b/DEFCLASS_$magic/g" -e "s/\bTHIS\b/THIS_$magic/g" -e "s/\bsaved_globals\b/saved_globals_$magic/g" -i class.final.sh
```