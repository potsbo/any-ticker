#ifndef _TICKER_MESSAGE_H_
#define _TICKER_MESSAGE_H_

class TickerMessage {
	public:
		std::string message;
		std::string fontName;
		int yAreaSize;
		int xAreaSize;
		TickerMessage() {
			message = "golly";
			fontName = "golly";
			yAreaSize = 11;
		}
		TickerMessage(std::string m, std::string f, int fontSize) {
			message = m;
			fontName = f;
			yAreaSize = 11;
		}
		void setDots(int dots[][256]);
		int calculateDistance(double bannerSize, int dotShift, int period);
	private:
		int xLeastAreaSize;
};

#endif // _TICKER_MESSAGE_H_
