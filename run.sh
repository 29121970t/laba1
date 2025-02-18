a="${1/.c/.o}"
echo $a
make $a
./$a "${@:2}"