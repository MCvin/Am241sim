#!/bin/sh

for seeds in $(seq  $1 1 $2); do 

    cat > autoSeeds$seeds.mac <<EOF
/random/setSeeds $seeds 1
/gps/particle gamma
/gps/pos/type Plane
/gps/pos/shape Circle
/gps/ang/type iso
/gps/pos/radius 3.75 mm
/gps/pos/centre 0. -4.825 85.00 cm  # scatter piece, unit 0
/gps/pos/rot2 0 0 1
/gps/ang/maxtheta 20. deg
/gps/ene/type Arb
/gps/hist/type arb
/gps/hist/file ../spectrum_Am241.dat
/gps/hist/inter	Lin
/run/beamOn 100000000
EOF

    echo "Running Geant4 with seed = $seeds"

    ./TestSim autoSeeds$seeds.mac ScatteredAm241_$seeds &

done
exit 0
