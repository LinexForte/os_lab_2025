#!/bin/bash
if [ $# -eq 0 ]; then
    echo "Нет аргументов"
    exit 1
fi
printf "%s\n" "$@" | awk '
{
    sum += $1
    n++
}
END {
    if (n > 0)
        printf "Количество: %d\nСреднее: %.2f\n", n, sum/n
    else
        print "Нет аргументов"
}'
