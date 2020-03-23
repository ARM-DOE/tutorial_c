#!/bin/sh

# Check for BUILD_PROJECT_NAME environment variable

name=$BUILD_PROJECT_NAME

if [ -z "$name" ]; then

    # Get 'project' from git repo URL

    name=`git config --get remote.origin.url | \
          sed 's/.*://;s/.git$//;s/^.*\///'`

    if [ -z "$name" ]; then

        # Use directory name for project name

        name=$(basename `pwd`)
    fi
fi

printf $name

exit 0
