
export SOURCE_PATH=/home/jason/make_patches/n-mr1-avko-nyi14/

#======================================================================================

export PATCH_TOP_PATH=$SOURCE_PATH/patches/
export PWD_PATH=`pwd`
export NEWPATCH_PATH=$PWD_PATH"/do_copy_folder/"

rm -Rf $NEWPATCH_PATH

for FILE_LIST in `find $PATCH_TOP_PATH -name *.patch | sort`
do
	PATCH_FILE=`cat $FILE_LIST  | grep "diff " | cut -d" " -f 3`
	#echo "1* "$PATCH_FILE
	
	for NEW_FILE_LIST in $PATCH_FILE
	do
		NEW_FILE=$SOURCE_PATH$NEW_FILE_LIST
		SOURCE_NEW_FILE=`echo $NEW_FILE | sed -r "s/\/a\//\//g"`
		DEST_NEW_FILE=$NEWPATCH_PATH$NEW_FILE_LIST
		DIR_NAME=`dirname $DEST_NEW_FILE`
		
		echo "=========================================="
		
		if [ ! -d $DIR_NAME ]; then
			echo "  mkdir -p "$DIR_NAME
			mkdir -p $DIR_NAME
		fi

		echo "cp "$SOURCE_NEW_FILE" "$DEST_NEW_FILE
		cp $SOURCE_NEW_FILE $DEST_NEW_FILE
	done
done

cp -a $PATCH_TOP_PATH $NEWPATCH_PATH"/a"
