#!/bin/sh

#===============================================================
#get line
[/root]# cat /proc/scsi/scsi | grep Vendor
  Vendor: ATA      Model: ST9500325AS      Rev: 0001
  Vendor: JetFlash Model: Transcend 2GB    Rev: 8.07
  Vendor: Kingston Model: DataTraveler 2.0 Rev: PMAP
cat /proc/scsi/scsi | grep Vendor | head -n 2 | tail -n 1
cat /proc/scsi/scsi | grep Vendor | awk 'NR==2'

Vendor: JetFlash Model: Transcend 2GB    Rev: 8.07

========================================
cat /etc/resolv.conf | head -n 2 | tail -n 1
¨ú2 3 4 (3)¦æ
cat /etc/passwd | head -n 4 | tail -n 3
------------------
sed -n '2,4p' /etc/passwd
------------------
awk 'NR>=2 && NR<=4 { print }' /etc/passwd

#===============================================================
#reciprocal 10 sec

count=10

while [ "$count" != "0" ]
do
    echo $count
    sleep 1
    count=$(( $(($count)) - 1))
done

#===============================================================
#use wget to check HDD
i=0
TOTAL=0
END_COUNT=48
MD5_ORG="6417e89b0ac91a7341cf7ef435929570"
URL="http://192.168.1.200/mmtest.img"
LOGFILE="/usr/app1/xfs_test.log"

echo `date` > $LOGFILE
echo "-------------------------------------------" >> $LOGFILE

while [ "$i" ]
do
    FILE="test_"$i".img"
    WGETCMD="wget $URL "-O" $FILE"
    $WGETCMD
    sync
    MD5_NEW=`md5sum $FILE | cut -c1-32`    
    
    i=$(( $(($i)) + 1))
    TOTAL=$(( $(($TOTAL)) + 1))
    echo "[AWAYSU] download time : "$TOTAL
    echo "[AWAYSU] download time : "$TOTAL >> $LOGFILE
    
    if [ "$MD5_ORG" != "$MD5_NEW" ] ; then
        echo "[AWAYSU] MD5 different!!"
        echo "[AWAYSU] MD5 different!!" >> $LOGFILE
        break
    fi
    
    if [ "$i" == "$END_COUNT" ] ; then         
        echo "-------------------------------------------"
        while [ "$i" != "0" ]
        do
            i=$(( $(($i)) - 1))
            FILE="test_"$i".img"
            sync
            MD5_NEW=`md5sum $FILE | cut -c1-32`
            echo "[AWAYSU] Check MD5 again : "$FILE
            if [ "$MD5_ORG" != "$MD5_NEW" ] ; then                 
                echo "[AWAYSU] check MD5 again different!!" >> $LOGFILE
                echo "[AWAYSU] check MD5 again different!!"
                break
            fi
        done       
        echo "-------------------------------------------"
        echo "[AWAYSU] kill all file & download again!!"
        rm test_* -rf
        #break
    fi
    
done

echo `date` >> $LOGFILE

#===============================================================
#loop run 2 app
BOOT_ID=`cat /config/boot.ini`
case "$BOOT_ID" in
    1) echo "run app1"
        echo "2" > /config/boot.ini
        /app1.sh
    ;;
    2) echo "run app2"
        echo "1" > /config/boot.ini
        /app2.sh
    ;;
esac

#===============================================================
#show path on #
export PS1=$'[\033[1m$PWD\033[0m]# '

#===============================================================
#get file size
size=`ls -l $filename | awk '{print $5}'`
size=`stat -c %s $filename`
size=$(stat -c %s $filename)

#===============================================================
#find mapping partition index
[root]#cat /proc/mtd
dev:    size   erasesize  name
mtd0: 00200000 00020000 "unuse0"
mtd1: 00000000 00020000 "bootloader"
mtd2: 00000000 00020000 "macadr"
mtd3: 00000000 00020000 "nvram"
mtd4: 00000000 00020000 "splash"
mtd5: 00000000 00020000 "unuse1"
mtd6: 00000000 00020000 "mtdinfo"
mtd7: 00500000 00020000 "kernel"
mtd8: 00f00000 00020000 "rootfs"
mtd9: 00200000 00020000 "config"
mtd10: 05b00000 00020000 "unalloc"

[root]#cat /proc/mtd | grep config | cut -d":" -f 1 | cut -d"d" -f 2
9


#===============================================================
#modify all txt file access 
DES_PATH=$1
#DES_PATH=`pwd`
EXECUTABLE_FILE_STR="ASCII"

for file_list in `find $DES_PATH/*`
do
    # check list is file or not
    if [ -f "$file_list" ]; then
        temp_exec_str=`file $file_list | grep "$EXECUTABLE_FILE_STR"`
        if [ "$temp_exec_str" != "" ]
        then
            echo "$file_list"
            chmod 666 $file_list
        fi
    fi
done


#===============================================================
#delete all CVS file
CVS_LIST=`find . -name CVS`
for file_list in $CVS_LIST
do
    rm -rf  "$file_list"
done

#===============================================================
#load config
#config.ini #TEST1=y

source config.ini
if [ "$TEST1" == "y" ]; then
    echo "TEST1!!"
fi

#===============================================================
#get current path
CURRENT_PATH=`pwd`

#===============================================================
#check file exist
FILE=abc
if [ -f $FILE ]
then
  echo "$FILE EXIST"
else
  echo "$FILE NOT EXIST"
fi

#===============================================================
#check folder exist
FOLDER=abc
if [ -x $FOLDER ]
then
  echo "$FOLDER EXIST"
else
  echo "$FOLDER NOT EXIST"
fi

