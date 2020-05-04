#ifndef __EZACCTIMER_H__
#define __EZACCTIMER_H__


class EzAccTimer
{
public:

	// Constructor
	EzAccTimer();

	void Start();
	void SetStartTime(unsigned __int32 time);
	unsigned __int32 Read() const;
	float ReadSec() const;

private:
	unsigned __int32	started_at = 0;
};

#endif //__EZACCTIMER_H__