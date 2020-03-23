#!/bin/sh

# Check for BUILD_GROUP_NAME environment variable

name=$BUILD_GROUP_NAME

if [ -z "$name" ]; then

    # Get 'group' from git repo URL

    name=`git config --get remote.origin.url | \
          sed 's/.*://;s/.git$//;s/\/.*$//'`

    if [ -z "$name" ]; then

        # Get group name from path to project

        path=`pwd`
        path=${path%/*}
        name=$(basename $path)
    fi
fi

printf $name

exit 0
