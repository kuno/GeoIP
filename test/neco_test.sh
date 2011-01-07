#!/usrbin/env sh

# Note: In order to use this testing script,
# You need install neco first,
# See detials at https://github.com/kuno/neco
source $HOME/utopia/.neco/activate.sh

report=./report.txt

if [ -e $report ]; then
  rm -rf $report
fi

# Tesing no nodejs 0.2.x stable branch
for neco_id in 'test0' 'test1' 'test2' 'test3' 'test4' 'test5' 'test6'; do
  neco_activate $neco_id &>/dev/null
  version=$(node -v)
  echo '#############################################################' >> $report
  echo '################# Testing on nodejs '$version' ##################' >> $report
  echo '#############################################################' >> $report
  sh test.sh >> $report
  echo '#############################################################' >> $report
  echo '############## Finished tests on nodejs '$version' ##############' >> $report
  echo '#############################################################' >> $report
  echo '' >> $report
  neco_deactivate
done

# Tesing no nodejs 0.3.x unstalbe branch
for neco_id in 'test10' 'test11' 'test12'; do
  neco_activate $neco_id &>/dev/null
  version=$(node -v)
  echo '#############################################################' >> $report
  echo '################## Testing on nodejs '$version' #################' >> $report
  echo '#############################################################' >> $report
  sh test.sh >> $report
  echo '#############################################################' >> $report
  echo '############## Finished tests on nodejs '$version' ##############' >> $report
  echo '#############################################################' >> $report
  echo '' >> $report
  neco_deactivate
done

# Tesing on latest version of nodejs
version=$(node -v)
echo '#############################################################' >> $report
echo '################# Testing on nodejs '$version' ##################' >> $report
echo '#############################################################' >> $report
sh test.sh >> $report
echo '#############################################################' >> $report
echo '############## Finished tests on nodejs '$version' ##############' >> $report
echo '#############################################################' >> $report
echo '' >> $report  
