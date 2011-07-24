#!/usr/bin/env sh

# Note: In order to use this testing script,
# You need install neco first,
# See detials at https://github.com/kuno/neco
source $NECO_ROOT/.neco/activate.sh

#report=./report.txt

rm -rf *.report

# Tesing no nodejs 0.2.x stable branch
#for neco_id in 'test0.2.1' 'test0.2.2' 'test0.2.3' 'test0.2.4' 'test0.2.5' 'test0.2.6'; do
#  neco_activate $neco_id &>/dev/null
#  version=$(node -v)
#  report=$version.report
#  echo "Testing on nodejs $version"
#  echo '#############################################################' >> $report
#  echo '################# Testing on nodejs '$version' ##################' >> $report
#  echo '#############################################################' >> $report
#  time sh test.sh >> $report
#  echo '#############################################################' >> $report
#  echo '############## Finished tests on nodejs '$version' ##############' >> $report
#  echo '#############################################################' >> $report
#  echo '' >> $report
#  neco_deactivate
#done

# Tesing no nodejs 0.3.x unstalbe branch
#for neco_id in 'test0.3.0' 'test0.3.1' 'test0.3.2' 'test0.3.3' 'test0.3.4' 'test0.3.5' 'test0.3.6' 'test0.3.7' 'test0.3.8'; do
#  neco_activate $neco_id &>/dev/null
#  version=$(node -v)
#  report=$version.report
#  echo "Testing on nodejs $version"
#  echo '#############################################################' >> $report
#  echo '################## Testing on nodejs '$version' #################' >> $report
#  echo '#############################################################' >> $report
#  time sh test.sh >> $report
#  echo '#############################################################' >> $report
#  echo '############## Finished tests on nodejs '$version' ##############' >> $report
#  echo '#############################################################' >> $report
#  echo '' >> $report
#  neco_deactivate
#done

# Tesing no nodejs 0.4.x unstalbe branch
for neco_id in 'test0.4.0' 'test0.4.1' 'test0.4.2' 'test0.4.3' 'test0.4.4' 'test0.4.5' 'test0.4.6' 'test0.4.7' 'test0.4.8'; do
  neco_activate $neco_id &>/dev/null
  version=$(node -v)
  report=$version.report
  echo "Testing on nodejs $version"
  echo '#############################################################' >> $report
  echo '################## Testing on nodejs '$version' #################' >> $report
  echo '#############################################################' >> $report
  time sh test.sh >> $report
  echo '#############################################################' >> $report
  echo '############## Finished tests on nodejs '$version' ##############' >> $report
  echo '#############################################################' >> $report
  echo '' >> $report
  neco_deactivate
  echo ''
done

# Remove testing building after tests are finished
rm -rf ../build/ || return 1
