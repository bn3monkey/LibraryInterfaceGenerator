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

# DICOM Comformance

Content Date	(0008,0023)	DA	Date of export
Content Time	(0008,0033)	TM	Time of export

Ultrasound Image send 할 때 이건 알아서 만들어서 보내자.
자동으로..

이건 보낼 때 정해져서 감
Specific Character Set	(0008,0005)	CS	“ISO_IR 100”	Always
SOP Class UID	(0008,0016)	UI	“1.2.840.10008.5.1.4.1.1.6.1”	Always
SOP Instance UID	(0008,0018)	UI	Generated automatically	Always


startExam

Patient Name : patient.name
Patient ID : patient.ID
patient's birth date : patient.birthDate
patient's sex : pateint.sex

referenced patinet sequence : []

performed procerdure step id : patient.id + timestamp

performed station ae title : MPPSServerConfiguration.applicationEntityTitle

performed station name : "SC1" (equipment.modelName)

performed location : null

performed procedure step start date : study.performedProcedureStepStartDate

perforemd procedure step start time : study.performedProcedureStepStartTime

performed procedure step status : study.status

performed procedure step description : study.performedProcedureStepDescription

performed procedure type description : null

procedure code sequence : null

Performed Procedure Step Discontinuation Reason Code Sequence : 

modality : study.series.modality(US)

study ID : study.id

performed series sequence
	performing physician's name : study.referringPhysician
	operator name : study.referringPhysician
	protocol name : equipment.protocolName
	series instauce uid : series.instanceUID
	series descrption : null
	referenced image seuqnece : image.ultrasoundsounds
		referenced sop class uid : 1.2.840.10008.5.1.4.1.1.6.1
		referenced sop instance uid : auto-generated
	Referenced Non-Image Composite SOP Instance Sequence : nullptr
		

Scheduled Step Attribute Sequence 
	study Instance UId : study.instanceUID
	referencedStudySequence : null,
	accession : study.accession
	requested procedure ID : study.requestedProcedure.id
	requestedProceduer Description : study.requestedProcedure.description
	scheduled Procedure STep ID : scheduledProcedure.id
	Scheduled Procedure Step Description : scheduledProcedure.description
	Scheduled Protocol Code Sequence : scheduledProcedure.protocolCode
		code value
		coding schme designator
		code meaning
t
endExam

performed procedure step status : study.status
performed procedure step end date : study.perfzormedProcedureStepEndDate
performed procedure step end time : study.performedProcedureStepEndTime
performed procduer step discontinuation reason code sequence :
	code value : 110500
	coding scheme designator : DCM
	code meaning : study.discontinuedReasons[i]


Performed Protocol Code Sq
	code value : worklist item scheduled protocol code sequence 
	coding scheme designator : same as above
	code meaning : same as above

