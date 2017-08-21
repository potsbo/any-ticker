#ifndef _TICKER_MESSAGE_H_
#define _TICKER_MESSAGE_H_

class TickerMessage {
	public:
		std::string message;
		TickerMessage(std::string m) {
			message = m;
		}
		TickerMessage(){
			message = "golly";
		}
};

#endif // _TICKER_MESSAGE_H_
