//aller anfang is schwer


class TimeEvent
{
	public:
		uint_16 timeleft;
		virtual void onEvent() = 0;
};
class HeapTimer
{
	private:
		TimerEvent[] Timer;
		int events;
	public:
		bool addTime(TimeEvent event);
}
HeapTimer::addTime(TimeEvent event)
{


}