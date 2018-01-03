#!/bin/bash
rm package.tar
tar cvf package.tar agent.conf QueryAntiVirusStatus.sh QuerySystemUpdates.sh ca-bundle.crt 
cd bin/Debug
tar rvf ../../package.tar ManagdAgent_Lin
cd ../../
aws s3 cp package.tar s3://managd/package_linux.tar --acl public-read 

