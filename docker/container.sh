#!/bin/bash
###################################################################
# Script Name:  container.sh
# Description:  Script to start up local container in interactive mode
# Args:         none
###################################################################

show_help() {
    echo "-----------------------------------------------------------------------"
    echo " This script provides easy commands to manage ADI Docker container.    "
    echo " See README-DOCKER.md for more information on the container.           "
    echo "-----------------------------------------------------------------------"
    echo "SYNTAX:   ./container.sh COMMAND"
    echo ""
    echo "COMMANDS:"
    echo "    start    Builds the container if it does not exist, starts the"
    echo "             container if is not already running, and opens an interactive"
    echo "             shell into the container.  Type 'exit' to exit the interacitve"
    echo "             shell."
    echo ""
    echo "    stop     Stops the running container."
    echo ""
    echo "    clean    Stops the container if it is running, and then deletes the"
    echo "             container and its associated image."
    echo ""
    exit 1
}

start() {
   # Make sure the container is started in the background.
   # If the container image does not exist, it will be built.
   cd ${docker_dir}
   docker-compose -p tutorial_c up -d

   # Start an interactive terminal for the container
   docker exec -it --user armdev7 tutorial_c bash
}

stop() {
   cd ${docker_dir}
   docker-compose -p tutorial_c stop
}

clean() {
   stop
   docker rm tutorial_c
   docker rmi tutorial_c

   # This is how we would clean a docker volume if it was created via docker-compose
   # process_name=$(echo "tutorial_c" | tr -d "_")
   # volume_name="${process_name}_arm_data"
   # docker volume rm ${volume_name}
}

# First get the directory where the script is located
docker_dir=`dirname "$0"`

argument="$1"
if [[ -z $argument ]] ; then
  show_help

else
  case $argument in
    start)
      start
      ;;
    stop)
      stop
      ;;
    clean)
      clean
      ;;
    *)
      show_help
      ;;
  esac
fi