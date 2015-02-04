# Make directories if necessary
mkdir -p ../Build/
mkdir -p ../Build/include
mkdir -p ../Build/lib
mkdir -p ../Build/ThirdPartyLibs

# Copy necessary files
cp -r ../ThirdPartyLibs/ ../Build/
cp ../Source/*.h* ../Build/include/

# Move the lib file to the lib folder
mv ./libBebopRebopAPI.a ../Build/lib/

rm ./Build/*.d ./Build/*.o 