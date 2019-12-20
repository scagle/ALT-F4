#!/bin/bash

FILES=`{ git ls-tree -r HEAD --name-only; git status | grep "new file" | awk '"'"'{ print $3 }'"'"'; } | grep -e "\.\(c\|cpp\)$" -e "\.\(h\|hpp\)$" -e "\.\(h\|c\)$" -e "\.\(h\|h\)$"| env LC_COLLATE=C sort`

if [[ $1 != "dry" ]]; then

    [[ ! -d "html/" ]] && mkdir "html"

    for FILE in $FILES
    do
        # will ignore any file in subdirectories that don't exist in html ( which is a blessing and a curse )
        nvim "$FILE" -c "colorscheme github" -c "TOhtml" -c "%s/1em/7/g" -c "%s/eeeeee/ffffff/g" -c "%s/monospace/"'"'"Courier New"'"'"/g" -c "w! html/$FILE.html" -c "qa!" 
    done
else
    echo "$FILES"
fi
