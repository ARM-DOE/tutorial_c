#!/bin/sh

# Check for BUILD_PACKAGE_NAME environment variable

name=$BUILD_PACKAGE_NAME

if [ -z "$name" ]; then

    # Get 'group-project' from git repo URL

    name=`git config --get remote.origin.url | \
          sed 's/.*://;s/.git$//;s/\//-/'`

    if [ -z "$name" ]; then

        # Get group name from path to project

        path=`pwd`
        path=${path%/*}
        group=$(basename $path)

        # Use directory name for project name

        project=$(basename `pwd`)

        name="$group-$project"
    fi
fi

printf $name

exit 0
