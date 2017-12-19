source class.final.sh

class Robot
    func Robot
    func raiseLeg
    func move
    func backfilp
    func getHeight
    func getWeight
    func setWeight
    func echo
    var height
    var weight

Robot::Robot() {
    height="$1"
    weight="$2"
}

Robot::echo() {
    printf "robot echo: $1\n"
}

Robot::setWeight() {
    weight=$1
}

Robot::getHeight() {
    echo "$height"
}

Robot::getWeight() {
    echo "$weight"
}

Robot::raiseLeg() {
    local which="$1"
    echo "raise the $which leg"
}

Robot::backfilp() {
    echo "backfilp"
}

Robot::move() {
    raiseLeg "left"
    raiseLeg "right"
}

const='1000'

# height=$const
# weight=$const

# create a Robot object and make it move
new Robot terminator 2M 500Kg

# height=$const
# weight=$const

terminator.move
terminator.setWeight 600Kg

height=$const
weight=$const

terminator.backfilp

# height=$const
# weight=$const

terminator.getHeight

# height=$const
# weight=$const

terminator.getWeight

if [ "$(typeof terminator)" != "Robot" ]; then printf "typeof not work!!!" && exit 1; fi

if [ "$(type -t echo)" != "builtin" ]; then printf "builtin echo is hide!!!" && exit 1; fi

# echo ${saved_values_2BA4876520844E069F7B429BB77F5ADE[@]}

# check if variable 'height' and 'weight' are unset
if [ -z ${height+x} ] || [ -z ${weight+x} ]; then
    if [ "$height" != "$const" ] || [ "$weight" != "$const" ]; then printf "global variable is hide!!!" && exit 1; fi
fi

echo 'passed'