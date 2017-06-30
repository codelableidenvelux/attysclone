#include "lowpass.h"

#include<qfont.h>
#include<stdio.h>


Lowpass::Lowpass(float _samplingrate, float cutoff) : QComboBox() {

	lp = new Iir::Butterworth::LowPass<LPORDER>;
	frequency = cutoff;
	samplingrate = _samplingrate;

	setMinimumWidth ( fontMetrics().width("10000Hz") );

	addItem(tr("off"),0);
	addItem(tr("1Hz"), 1);
	addItem(tr("2Hz"), 2);
	addItem(tr("5Hz"), 5);
	addItem(tr("10Hz"),10);
	addItem(tr("20Hz"), 20);
	addItem(tr("50Hz"), 50);
	addItem(tr("||1Hz"), -1);
	addItem(tr("||2Hz"), -2);
	addItem(tr("||5Hz"), -5);

	connect(this,
		SIGNAL( activated(int) ),
		this,
		SLOT( setFrequencyIndex(int) ) );

	setFrequency(cutoff);
}

void Lowpass::setFrequencyIndex ( int index ) {
 	frequency = (float)(itemData(index).toFloat());
	if (fabs(frequency) > 0) {
		lp->setup(LPORDER,
			samplingrate,
			(float)fabs(frequency));
		//_RPT2(0, "lowpass=%fHz,idx=%d\n", frequency,index);
		lp->reset();
	}
}

void Lowpass::setFrequency(float f) {
	//_RPT1(0, "setFre lowpass=%fHz\n", f);
	int index = findData(f);
	if (index != -1) {
		setCurrentIndex(index);
		setFrequencyIndex(index);
	}
}

float Lowpass::filter(float v) {
	if (frequency > 0) {
		return lp->filter(v);
	}
	else {
		if (frequency < 0) {
			return lp->filter(fabs(v));
		}
	}
	return v;
}
