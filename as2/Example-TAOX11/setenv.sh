# Define TAOX11 installatin directory
export TAOX11_BASE_ROOT=~/taox11/

# Work around library naming
ln -fs /usr/lib64/libcrypto.so libcrypto.so.10
export LD_LIBRARY_PATH=".:${TAOX11_BASE_ROOT}/lib/:${TAOX11_BASE_ROOT}/ACE/ACE/lib"


