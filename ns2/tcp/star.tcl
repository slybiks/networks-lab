set ns [new Simulator]
set nf [open out.nam w]
$ns color 1 Blue
$ns color 2 Red
$ns namtrace-all $nf
 
proc finish {} {
    global ns nf
    $ns flush-trace
    close $nf
    exec nam out.nam &
    exit 0
}
 
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
 
$ns duplex-link $n0 $n1 2Mb 10ms DropTail
$ns duplex-link $n0 $n2 2Mb 10ms DropTail
$ns duplex-link $n0 $n3 2Mb 10ms DropTail
$ns duplex-link $n0 $n4 2Mb 10ms DropTail
 
$ns duplex-link-op $n0 $n1 orient right-up
$ns duplex-link-op $n0 $n2 orient left-up
$ns duplex-link-op $n0 $n3 orient right-down
$ns duplex-link-op $n0 $n4 orient left-down

set tcp [new Agent/TCP]
$tcp set class_ 1
$ns attach-agent $n0 $tcp
 
set sink [new Agent/TCPSink]
$ns attach-agent $n4 $sink
$ns attach-agent $n3 $sink 
$ns attach-agent $n2 $sink
$ns attach-agent $n1 $sink 
$ns connect $tcp $sink
 
# Setup a CBR over TCP connection
set cbr [new Application/Traffic/CBR]
$cbr attach-agent $tcp
$cbr set type_ CBR
$cbr set packet_size_ 1000
$cbr set rate_ 1mb
$cbr set random_ false
 
 
# Schedule events for the CBR agents
$ns at 0.1 "$cbr start"
$ns at 4.5 "$cbr stop"
 
# Detach tcp and sink agents
$ns at 4.5 "$ns detach-agent $n0 $tcp ; $ns detach-agent $n1 $sink ; $ns detach-agent $n2 $sink ; $ns detach-agent $n3 $sink ; $ns detach-agent $n4 $sink"
 
# Call the finish procedure after
# 5 seconds of simulation time
$ns at 5.0 "finish"
 
# Print CBR packet size and interval
puts "CBR packet size = [$cbr set packet_size_]"
puts "CBR interval = [$cbr set interval_]"
 
# Run the simulation
$ns run
