#ifndef __EZACCPERFTIMER_H__
#define __EZACCPERFTIMER_H__



class EzAccPerfTimer
{
public:

	// Constructor
	EzAccPerfTimer();

	void Start();
	double ReadMs() const;
	unsigned __int64 ReadTicks() const;

private:
	unsigned __int64	started_at = 0;
	static unsigned __int64 frequency;
};

#endif //__EZACCPERFTIMER_H__