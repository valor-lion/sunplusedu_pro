
#cd /home/fm_radio

echo "insmod the fm_radio driver...."
insmod S3C2440_si4702.ko
echo "insmod done!"

echo "starting fm_radio [Qt] program...."
./FMQt -qws &

