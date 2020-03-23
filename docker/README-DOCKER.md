# ADI Container for tutorial_c
This folder contains the files required to configure a Docker container for developing and
testing the tutorial_c process.  This container provides an out of the box configured
ADI environment that is ready to run.  

# Prerequisites:
### 1. Install Git
*  [Click here to install Git](https://git-scm.com/downloads)
* To test whether `git` is installed on your computer, run the following command:

   ```
   git --version
   ```

* If successful, you should see the version number of git. For example:

   ```
   git --version  

   git version 2.17.2 (Apple Git-113)
   ```

### 2. Install Docker

* [Click here to install Docker](https://docs.docker.com/install/)
* Test that your docker installation works by running the simple Docker image, [hello-world](https://hub.docker.com/_/hello-world/)

   ```
   docker run hello-world
   ```

   You should see the following output:

   ```
   docker run hello-world

   Unable to find image 'hello-world:latest' locally
   latest: Pulling from library/hello-world
   1b930d010525: Already exists 
   Digest: sha256:6540fc08ee6e6b7b63468dc3317e3303aae178cb8a45ed3123180328bcc1d20f
   Status: Downloaded newer image for hello-world:latest
   
   Hello from Docker!
   This message shows that your installation appears to be working correctly.

   To generate this message, Docker took the following steps:
    1. The Docker client contacted the Docker daemon.
    2. The Docker daemon pulled the "hello-world" image from the Docker Hub.
       (amd64)
    3. The Docker daemon created a new container from that image which runs the
       executable that produces the output you are currently reading.
    4. The Docker daemon streamed that output to the Docker client, which sent it
       to your terminal.
   
   To try something more ambitious, you can run an Ubuntu container with:
    $ docker run -it ubuntu bash
   
   Share images, automate workflows, and more with a free Docker ID:
    https://hub.docker.com/
   
   For more examples and ideas, visit:
    https://docs.docker.com/get-started/
   ```

* Check your docker version:

   ```
   docker --version
   
   Docker version 19.03.1, build 74b1e89
   ```

   _For this demo, your docker version needs to be at least **`18.06.0`**. If your docker verion is older, update to the latest version._

   More info about getting started with Docker can be found at: [https://docs.docker.com/get-started/](https://docs.docker.com/get-started/)


### 4. Install docker-compose (LINUX ONLY).
  
* For Linux, follow the [Install Compose](https://docs.docker.com/compose/install/#install-compose) instructions for Linux systems.

   _(For Mac and Windows, docker-compose is included as part of the Docker Desktop installation.)_

* Test docker-compose is installed.

   ```
   docker-compose --version
   
   docker-compose version 1.24.1, build 4667896b
   ```

### 5. Configure docker to run on Windows Subsystem for Linux (WSL) - (WINDOWS 10 ONLY).
  
* If you are running on windows 10, then you should have installed Docker Desktop from docker.com as defined in 
  step 2 above.

* However in order to run linux command shell scripts for docker, you also need to install Windows Subsytem for Linux (WSL).
  You can install WSL from these instructions: https://www.windowscentral.com/install-windows-subsystem-linux-windows-10.

  Select the **```Ubunto 18.04  LTS```** distribution

* Once you have WSL installed, then you need to add a docker command-line client to WSL that can talk to the docker
  daemon running in Docker Desktop.  The following instructions show you how to do this:
  
  https://nickjanetakis.com/blog/setting-up-docker-for-windows-and-wsl-to-work-flawlessly
  
    .


# To run tutorial_c in your container, perform the following steps:
1. ###  Clone the tutorial_c project from git into a folder on your local machine:
    ```
    git clone https://code.primre.org/examples/tutorial_c.git
    ```

2. ### Initialize your container environment variables:
    *Note that this only needs to be done the first time you use the container.*  
    ```
    cd tutorial_c/docker
    cp .env.default .env
    ```
  
    If desired, change the values in the .env file *(should not be necessary unless you are running multiple ADI 
    containers)*

3. ### Start the container: 
    
    ```
    ./container.sh start
    ```

    This will start and log you in to the container as the `armdev7` user.
    You should see the following on your command line:

    ```
    [armdev7@tutorial_c ~]$
    ```

    See the ```container.sh``` section below for more info  
  
4. ### Build your process code   
    
    ```
    cdcode   <--- this alias goes here:  /home/armdev7/code.arm.gov/vap/tutorial_c   
    ./build.sh
    ```
    
4. ### Run dsproc_test
     
    ```
    cdcode
    cd test
    dsproc_test
    ```
.

---
# More information about dsproc_test:
**dsproc_test** will run ADI against the test scenarios specified in the dsproc_test config file 
(`/home/armdev7/code.arm.gov/vap/tutorial_c/test/dsproc_test.cfg`).  
When dsproc_test runs, it performs the following steps:

1) Runs the tutorial_c process (based on ADI) for a given site, facility, and time range
2) Dumps the ouput netcdf file to a text file
3) Diffs the netcdf dump file with a pre-computed dump file that is known to be correct.  If there are no differences,
the test is a success.

For this particular example, the following three runs are conducted:
```
===========================================================================
STARTING PROCESS: tutorial_c VAP At nsaC1 From 20140101 to 20140102
===========================================================================
===========================================================================
STARTING PROCESS: tutorial_c VAP At sgpC1 From 20140101 to 20140102
===========================================================================
===========================================================================
STARTING PROCESS: tutorial_c VAP At sgpE13 From 20140101 to 20140102
===========================================================================
```

### Transformation configuration
The tutorial_c process combines several different datastreams onto a common coordinate grid.  The configuration specifying
which datastreams/variables to combine and how to transform them is contained in a local database located at:

```
/home/armdev7/db/dsdb.sqlite
```

In addition, a json version of this configuration is stored in a file located at: 
```
/home/armdev7/code.arm.gov/vap/tutorial_c/conf/tutorial_c.process

```

### Input Data
For the nsaC1 run, all the input datastream files are located in the following 3 folders:
```
/data/home/dev/vap/tutorial_c/DATA/data/datastream/nsa/nsaceilC1.b1
/data/home/dev/vap/tutorial_c/DATA/data/datastream/nsa/nsametC1.b1
/data/home/dev/vap/tutorial_c/DATA/data/datastream/nsa/nsatpsC1.b1
```

For the sgpC1 & sgpE13 runs, all the input datastream files are located in the following 3 folders:
```
/data/home/dev/vap/tutorial_c/DATA/data/datastream/sgp/sgp1twrmrC1.c1
/data/home/dev/vap/tutorial_c/DATA/data/datastream/sgp/sgpceilC1.b1
/data/home/dev/vap/tutorial_c/DATA/data/datastream/sgp/sgpmetE13.b1

```       

### Output Data
Each run of tutorial_c produces two different datastreams, demonstrating transforming onto two different coordinate grids.
The `tutorial` files have 60s time interval, whereas the `tutorial30s` files have a 30s time interval.
**The output netcdf files are located in the following folders:**

nsaC1
```
/data/home/dev/vap/tutorial_c/DATA/data/datastream/nsa/nsatutorialC1.c1
/data/home/dev/vap/tutorial_c/DATA/data/datastream/nsa/nsatutorial30sC1.c1

```  
sgpC1
```
/data/home/dev/vap/tutorial_c/DATA/data/datastream/sgp/sgptutorialC1.c1
/data/home/dev/vap/tutorial_c/DATA/data/datastream/sgp/sgptutorial30sC1.c1

```  
sgpE13
```
/data/home/dev/vap/tutorial_c/DATA/data/datastream/sgp/sgptutorialE13.c1
/data/home/dev/vap/tutorial_c/DATA/data/datastream/sgp/sgptutorial30sE13.c1

```  

**The ncdump files of the output are located in:**
```
/data/home/dev/vap/tutorial_c/DATA/out/*.dump
``` 

**The expected ncdump files (i.e., reference data) for the output are located in:**
```
/data/home/dev/vap/tutorial_c/DATA/ref/*.dump
``` 

**The diff between the expected ncdump files and the output ncdump files are located in:**
```
/data/home/dev/vap/tutorial_c/DATA/out/*.diff
``` 

### Process Logs
When tutorial_c is run, ADI keeps a detailed log of the steps being performed in the transformation.  A separate
log file is created for each different site/facility detailed.  Log files for the nsaC1, sgpC1, and sgpE13 runs
can be found in the following folders, respectively:
```
/data/home/dev/vap/tutorial_c/DATA/data/logs/nsa/proc_logs/nsatutorial_cC1/nsatutorial_cC1.*.VAP
/data/home/dev/vap/tutorial_c/DATA/data/logs/sgp/proc_logs/sgptutorial_cC1/sgptutorial_cC1.*.VAP
/data/home/dev/vap/tutorial_c/DATA/data/logs/sgp/proc_logs/sgptutorial_cE13/sgptutorial_cE13.*.VAP
``` 

.

---
# The following sections contain more information about the container:

### container.sh script
```
    -----------------------------------------------------------------------
     This script provides easy commands to manage ADI Docker container.    
     See README-DOCKER.md for more information on the container.           
    -----------------------------------------------------------------------
    SYNTAX:   ./container.sh COMMAND
    
    COMMANDS:
        start    Builds the container if it does not exist, starts the
                 container if is not already running, and opens an interactive
                 shell into the container.  Type 'exit' to exit the interacitve
                 shell.
    
        stop     Stops the running container.
    
        clean    Stops the container if it is running, and then deletes the
                 container and its associated image.
```
.

---
### Container environment
##### User
- The container is running as user `armdev7`.

##### Mounted Folders
- The container's **`/home/armdev7/code.arm.gov/vap/tutorial_c`** folder is automatically mounted
to your code project (../../tutorial_c).
  
##### Environment Variables
- All the proper ADI environment variables have been set and do not need to be changed.  However you can 
extend the /home/armdev7/.bashrc file if you need to add new settings or aliases.
- Type the ```alias``` command to see the list of default aliases.

##### Database
- When the container is first created, it will automatically download a sqlite database for your process which is stored
in ```/home/armdev7/db/db.sqlite```.
- When created, this databse will reflect the current state of the information stored in the PCM.
- **NOTE** that this means that you must have internet access when your container is first created or else
it will fail.  Once the container is created, internet access is no longer required.
- To update the database on your container in the case that changes have been made to your
process or DODs in the PCM, run ```container.sh clean``` and then ```container.sh start```

##### Data
-  This container comes pre-populated with the appropriate data required to run dsproc_test.  Test data is located
at ```/data/home/dev/vap/tutorial_/DATA```

---
### Software Development
Since your source code is mounted into the container, you should be editing the code from your local
machine using your favorite editor or IDE.  The container shell is meant for running and debugging the process.


---
### Container Customization
For most cases you should not need to modify anything in your Docker container.  However, if you 
require custom dependencies in your process, you can add them to the adjacent Dockerfile 
and then clean and restart your container.  For more information on Docker, 
please see these [tutorials](https://hackr.io/tutorials/learn-docker "Docker Tutorials").

---
### SSH
SSH can be used by your favorite IDE to connect to the container's language environment.  You may
also use an ssh terminal as an alternative to the docker interactive shell. **The ssh port that
your container uses is specified in the adjacent `.env` file.**  The default value is

```
SSH_PORT=56269
```

 - **NOTE:  If you plan to run more than one process container at a time, you will need to change this value
to avoid a port conflict.**

To ssh to your running container, use the following command

```
ssh -p 56269 armdev7@localhost
```

where the password is `armdev7`.
