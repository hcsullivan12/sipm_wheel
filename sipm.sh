# Must supply directory containing SiPM data
WHEEL_DIR=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )

export SIPMS_TO_CHARACTERIZE_DIR="$WHEEL_DIR/sipms_to_characterize1"
echo $SIPMS_TO_CHARACTERIZE_DIR

# Loop over sipms
for d in $(find $WHEEL_DIR/data/$SIPMS_TO_CHARACTERIZE_DIR -mindepth 1 -maxdepth 1 -type d)
do
  
	echo "Analyzing " $d
	
	#root -l 'test.C()'

	done;

echo 
echo

#root -l CharacterizeSiPMs.C

echo "GOT TERMINAL"
