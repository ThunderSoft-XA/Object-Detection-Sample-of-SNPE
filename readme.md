# Object Detect Algorithm Conversion and ObjectDetectionSample Developer documentation

## brief introduction

### This document mainly introduces the deployment and use of ObjectDetectionSample environment。

## Transform object detection model into DLC file

#### 1. Tensorflow set up

##### Instructions for TensorFlow setup are https://www.tensorflow.org/get_started/os_setup. Follow instructions to install TensorFlow on Ubuntu Linux.The SNPE SDK has been tested with TensorFlow v1.6

#### 2.Python 3.5

##### "/usr/bin/python" and "python" should point to Python 3. It can be achieved using following steps :

###### Create list of alternatives for python

```
          $ sudo update-alternatives --install /usr/bin/python python /usr/bin/python2.7 1
          $ sudo update-alternatives --install /usr/bin/python python /usr/bin/python3.5 2

      Check the python list
          $ update-alternatives --list python

      update alternative to select required python version
          $ sudo update-alternatives --config python
```

#### 3.Setup SDK Environment and Dependencies

  Download the sdk from qualcomm website Extract the SDK to a directory. The directory the SDK is extracted to is referred to as $SNPE_ROOT. Note the use of "X.Y.Z" in this section. It  refers to the release zip that is in use (eg. snpe-1.0.0.zip). unzip -X snpe-X.Y.Z.zip Run the dependencies script to check the system for Ubuntu package  dependencies. It will ask to install ones that are missing. Install the  missing packages. source snpe-X.Y.Z/bin/dependencies.sh Run the python dependency checker to check the system for python package dependencies. Install the missing packages.

source snpe-X.Y.Z/bin/check_python_depends.sh

#### 4.Envionment setup for TensorFlow

  Using the SNPE SDK with TensorFlow models requires a valid  TensorFlow installation (see TensorFlow Setup.) Go to $SNPE_ROOT and run the following script to setup the SNPE SDK  environment. The location of the TensorFlow directory must be provided. source bin/envsetup.sh -t $TENSORFLOW_DIR where $TENSORFLOW_DIR is the path to the TensorFlow installation. The script sets up the following environment variables.

 SNPE_ROOT: root directory of the SNPE SDK installation TENSORFLOW_HOME: root directory of the TensorFlow installation provided The script also updates PATH, LD_LIBRARY_PATH, and PYTHONPATH.

#### 5. get Inception v3

   download file from this website- https://storage.googleapis.com/download.tensorflow.org/models/inception_v3_2016_08_28_frozen.pb.tar.gz

#### 6. trans model file

   The following command will convert an Inception v3 TensorFlow  model into a SNPE DLC file.   snpe-tensorflow-to-dlc --input_network  $SNPE_ROOT/models/inception_v3/tensorflow/inception_v3_2016_08_28_frozen.pb                       --input_dim input "1,299,299,3" --out_node  "InceptionV3/Predictions/Reshape_1" --output_path inception_v3.dlc                       --allow_unconsumed_nodes

### Note: the detail of the steps depend on your own envionment.

\## 

------

## usage method

### 1.Installing the ADB tool

### 2.Push the whole Project to the target devices

### 3.create a debug directory under the root of project

### 4.build the project into debug file

### 5.the executable file