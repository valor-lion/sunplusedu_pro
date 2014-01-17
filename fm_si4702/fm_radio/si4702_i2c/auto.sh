rm -rf /lib/modules/2.6.35.7/S3C2440_si4702.ko
cp -a S3C2440_si4702.ko /lib/modules/2.6.35.7/ -rf
insmod S3C2440_si4702.ko
rm -rf /lib/modules/2.6.35.7/modules.dep.bb
