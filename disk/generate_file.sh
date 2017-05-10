
echo "generate $1 MB files, pattern: $2 "

let cnt=$1*1024*1024/4

for i in $(seq 1 1 $cnt)
do

echo -ne "\x55\xaa\x55\xaa" >> file.img

done

md5sum file.img >> file_md5sum.txt

