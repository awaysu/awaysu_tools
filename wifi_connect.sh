#!/bin/sh
#
#	wrote by awaysu 2018.1.19
#

INTERFACE="wlan0"
WIFI_CONNECT_LOG="/var/wifi_connect.log"
SCAN_RESULTS="/var/connect_ssid.txt"
WIFI_EXEC_PROC="/sbin/wl"
STR_COMMAND="NULL"

connect_ap()
{
    $WIFI_EXEC_PROC down
    $WIFI_EXEC_PROC up
    $WIFI_EXEC_PROC auth 0
    $WIFI_EXEC_PROC infra 1

    if [ "$STR_COMMAND" == "OPEN" ]; then
        $WIFI_EXEC_PROC wsec 0
    elif [ "$STR_COMMAND" == "WEP" ]; then
        $WIFI_EXEC_PROC wsec 1
    elif [ "$STR_COMMAND" == "WPA-TKIP" ] || [ "$STR_COMMAND" == "WPA2-TKIP" ]; then
        $WIFI_EXEC_PROC wsec 2
    elif [ "$STR_COMMAND" == "WPA-AES" ] || [ "$STR_COMMAND" == "WPA2-AES" ]; then
        $WIFI_EXEC_PROC wsec 4
    fi

    $WIFI_EXEC_PROC sup_wpa 1

    if [ "$STR_COMMAND" == "WEP" ] || [ "$STR_COMMAND" == "OPEN" ]; then
        $WIFI_EXEC_PROC wpa_auth 0
    elif [ "$STR_COMMAND" == "WPA-TKIP" ] || [ "$STR_COMMAND" == "WPA-AES" ]; then
        $WIFI_EXEC_PROC wpa_auth 4
    elif [ "$STR_COMMAND" == "WPA2-TKIP" ] || [ "$STR_COMMAND" == "WPA2-AES" ]; then
        $WIFI_EXEC_PROC wpa_auth 128
    fi

    if [ "$STR_COMMAND" == "WEP" ]; then
        $WIFI_EXEC_PROC addwep 0 $2
    elif [ "$STR_COMMAND" != "OPEN" ]; then
        $WIFI_EXEC_PROC set_pmk $2
    fi

    RET=$?
    #echo $RET

    if [ "$RET" != "0" ]; then
       echo "Passphrase maybe error" | tee -a $WIFI_CONNECT_LOG
       exit 1
    fi

    $WIFI_EXEC_PROC ssid $1

    # wait for the dongle linking to the associated AP
    echo -n "[Associating]" | tee -a $WIFI_CONNECT_LOG
    STATUS="Not associated"
    RETRY=0
    sleep 5

    while [ "$STATUS" == "Not associated"  ]; do
        sleep 2
        STATUS=`$WIFI_EXEC_PROC status | grep associated | cut -d"." -f 1`
        RETRY=`expr $RETRY + 1`
        echo -e ".\c"
        if [ "$RETRY" == "6" ]; then
            echo "The associtated AP is not available" | tee -a $WIFI_CONNECT_LOG
            exit 1
        fi
    done

    BSSID=`$WIFI_EXEC_PROC status | grep BSSID | cut -c 8-24`

    if [ "$BSSID" == "00:00:00:00:00:00" ]; then
        echo "The associtated AP is empty." | tee -a $WIFI_CONNECT_LOG
        exit 1
    elif [ "$STR_COMMAND" != "OPEN" ] && [ "$STR_COMMAND" != "WEP" ]; then
		RETRY=0

		while [ "$AUTHORIZED" != "AUTHORIZED"  ]; do
			sleep 1
			AUTHORIZED=`$WIFI_EXEC_PROC sta_info $BSSID | grep state | cut -d":" -f 2 | cut -d" " -f 4`
			RETRY=`expr $RETRY + 1`
			echo -e ".\c"
			if [ "$RETRY" == "3" ]; then
                echo "Passphrase maybe error" | tee -a $WIFI_CONNECT_LOG
                exit 1
			fi
		done
		echo
    fi
}

# ------------------ Start ------------------

rm -f $WIFI_CONNECT_LOG
if [ -z "$1" ]; then
    echo "Usage: $0 <SSID>" 
	echo "       $0 <SSID> <PASSWORD>" 
	echo
	exit 1
fi

$WIFI_EXEC_PROC down
$WIFI_EXEC_PROC up
#$WIFI_EXEC_PROC scan --ssid=$1
rm -f $SCAN_RESULTS

echo -n "[Scan SSID:$1]" | tee -a $WIFI_CONNECT_LOG
#sleep 5

RETRY=0
until [ -s $SCAN_RESULTS ]; do
	rm -f $SCAN_RESULTS		
	$WIFI_EXEC_PROC scan --ssid=$1
	echo -n "."
	sleep 4
	$WIFI_EXEC_PROC scanresults > $SCAN_RESULTS
	RETRY=`expr $RETRY + 1`
	
	sleep 1
	
	if [ "$RETRY" == "5" ]; then
		rm -f $SCAN_RESULTS
		echo "Can't get the results of the specific SSID" | tee -a $WIFI_CONNECT_LOG
		exit 1
	fi
done

echo

if [ ! -s $SCAN_RESULTS ]; then
    echo "Scan SSID Error" | tee -a $WIFI_CONNECT_LOG
    exit 1
fi

# ------------------ check AP mode ------------------
STR_COMMAND="OPEN"
STR_MODE=`cat $SCAN_RESULTS | grep WPA2`
if [ "$STR_MODE" == "" ]; then
	STR_MODE=`cat $SCAN_RESULTS | grep "WPA"`
	if [ "$STR_MODE" != "" ]; then
		STR_COMMAND="WPA"
	else
		STR_MODE=`cat $SCAN_RESULTS | grep "WEP"`
		if [ "$STR_MODE" != "" ]; then
			STR_COMMAND="WEP"
		fi
	fi
else
	STR_COMMAND="WPA2"
fi

if [ "$STR_COMMAND" == "WPA2" ] || [ "$STR_COMMAND" == "WPA" ]; then
	STR_MODE=`cat $SCAN_RESULTS | grep TKIP`
	
	if [ "$STR_MODE" != "" ]; then
		STR_COMMAND=$STR_COMMAND"-TKIP"
	else
		STR_COMMAND=$STR_COMMAND"-AES"
	fi
fi

#echo "========================= debug ======================"
#cat $SCAN_RESULTS
echo "=> AP Mode : "$STR_COMMAND

# ------------------ connect to AP ------------------
connect_ap $1 $2

# ------------------ get ip from hdcp ------------------
/sbin/udhcpc -i $INTERFACE
echo ""
ifconfig $INTERFACE

