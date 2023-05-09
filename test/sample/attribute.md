# RegionSpatialFormat

0000H
None or not applicable

0001H
2D (tissue or flow)

0002H
M-Mode (tissue or flow)

0003H
Spectral (CW or PW Doppler)

0004H
Wave form (physiological traces, Doppler traces,…)

0005H
Graphics

# Region Data Type

0000H
None or not applicable

0001H
Tissue

0002H
Color Flow

0003H
PW Spectral Doppler

0004H
CW Spectral Doppler

0005H
Doppler Mean Trace

0006H
Doppler Mode Trace

0007H
Doppler Max Trace

0008H
Volume Trace

000AH
ECG Trace

000BH
Pulse Trace

000CH
Phonocardiogram Trace

000DH
Gray bar

000EH
Color bar

000FH
Integrated Backscatter

0010H
Area Trace

0011H
d(area)/dt

0012H
Other Physiological (Amplitude vs. Time) input

# Region Flags Attribute 

Enumerated Values for Bit 0 (lsb):

0
Region pixels are high priority

1
Region pixels are low priority


Enumerated Values for Bit 1 Scaling Protection:

0
Not Protected
1
Protected

Enumerated Values for Bit 2 Doppler Scale Type:

0
Velocity
1
Frequency

Enumerated Values for Bit 3-4 Scrolling Region:

00
Unspecified

01
Scrolling

10
Sweeping

11
Sweeping then Scrolling

Bit 5-31 Reserved for future use, shall be set to zero.

# PhysicalUnitsXDirection

Enumerated Values:

0000H
None or not applicable

0001H
Percent

0002H
dB

0003H
cm

0004H
seconds

0005H
hertz(seconds-1)

0006H
dB/seconds

0007H
cm/sec

0008H
cm2

0009H
cm2/sec

000AH
cm3

000BH
cm3/sec

000CH
degrees