#/usr/bash

# We check in user library
# if conventional jansson library was 
# already installed on the system
janssonlib_files=$(ls /usr/lib/ | grep libjansson.so)
janssonLibInstalled=1

if [ -z "$janssonlib_files" ] ; then
    janssonLibInstalled=0
fi

if [ $janssonLibInstalled == 1 ] ; then
    janssonlib_url="http://www.digip.org/jansson/releases/jansson-2.12.tar.bz2"

    echo "Jansson library not found on your system"
    echo "Jansson library will be downloaded from '$janssonlib_url'"
    echo "the installed in the /usr/lib directory"

    wget http://www.digip.org/jansson/releases/jansson-2.12.tar.bz2

    download_state=$?

    if [ $download_state != 0 ] ; then
        echo "Failed to download jansson library, please check the url"
    else

        bunzip2 -c jansson-2.12.tar.bz2 | tar xf -

        unzip_state=$?

        if [ $unzip_state != 0 ] ; then
            echo "Failed to unzip 'jansson-2.12.tar.bz2'"
        else

            cd jansson-2.12

            ./configure
            make
            make check
            make install

            cd ../

            rm jansson-2.12.tar.bz2
            rm -r jansson-2.12

            janssonLibInstalled=1

        fi
    
    fi

    echo "jansson library dependency installed !"
else
    echo "jansson library already installed, all is fine"
fi

if [ $janssonLibInstalled == 1 ] ; then
    make
    make clear
    cp bin/codegen-cli /usr/bin

    echo "codegen was installed with success"
else
    echo "Jansson library failed to install, codegen install can't ne performed"
fi