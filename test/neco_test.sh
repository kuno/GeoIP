source $HOME/utopia/.neco/activate.sh

report=./report.txt

if [ -e $report ]; then
  rm -rf $report
fi

neco_activate test0 > /dev/null
echo '>>>>>>>>>>>>>>>>>>>>Testing on '$ecosystem'<<<<<<<<<<<<<<<<<<<' >> $report
node -v >> $report
sh test.sh >> $report
echo '>>>>>>>>>>>>>>>>>>>>Finished Testing on '$ecosystem '<<<<<<<<<<<<<<<<<<<<<<' >> $report
neco_deactivate

neco_activate test1 > /dev/null
echo '>>>>>>>>>>>>>>>>>>>>Testing on '$ecosystem'<<<<<<<<<<<<<<<<<<<' >> $report
node -v >> $report
sh test.sh >> $report
echo '>>>>>>>>>>>>>>>>>>>>Finished Testing on '$ecosystem '<<<<<<<<<<<<<<<<<<<<<<' >> $report
neco_deactivate          
