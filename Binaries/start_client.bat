@echo OFF
cd ../Dependencies && copy pthreadVCE2.dll ..\Binaries
cd ../Dependencies/Ogre3D && "../../Binaries/NNYv3 Client.exe"
del ..\..\Binaries\pthreadVCE2.dll