SHELL       = /bin/sh
GOA         = ../..
MAKEFLAGS	= -j4

all:
	cd $(GOA)/Chat/chatc/chatlinux/ ; $(MAKE) clean && $(MAKE)
	cd $(GOA)/gcdkey/gcdkeylinux/gcdkeyclientlinux ; $(MAKE) clean && $(MAKE)
	cd $(GOA)/gcdkey/gcdkeylinux/gcdkeyserverlinux ; $(MAKE) clean && $(MAKE)
	cd $(GOA)/ghttp/ghttpc/ghttplinux/ ; $(MAKE) clean && $(MAKE)
	cd $(GOA)/GP/gptestc/gplinux ; $(MAKE) clean && $(MAKE)
	cd $(GOA)/gstats/statstest/gstatslinux ; $(MAKE) clean && $(MAKE)
	cd $(GOA)/gstats/persisttest/persistlinux ; $(MAKE) clean && $(MAKE)
	cd $(GOA)/gt2/gt2testc/gt2linux ; $(MAKE) clean && $(MAKE)
	cd $(GOA)/natneg/simpletest/natneglinux ; $(MAKE) clean && $(MAKE)
	cd $(GOA)/Peer/peerc/peerlinux ; $(MAKE) clean && $(MAKE)
	cd $(GOA)/pt/pttestc/ptlinux ; $(MAKE) clean && $(MAKE)
	cd $(GOA)/qr2/qr2csample/qr2linux ; $(MAKE) clean && $(MAKE)
	cd $(GOA)/serverbrowsing/sbctest/sblinux ; $(MAKE) clean && $(MAKE)
#	cd $(GOA)/Voice/voicetest/voicetestlinux ; $(MAKE) clean && $(MAKE)
#	cd $(GOA)/Voice/voicebench/voicebenchlinux ; $(MAKE) clean && $(MAKE)

.PHONY : clean
clean:
	cd $(GOA)/Chat/chatc/chatlinux/ ; $(MAKE) clean
	cd $(GOA)/gcdkey/gcdkeylinux/gcdkeyclientlinux ; $(MAKE) clean
	cd $(GOA)/gcdkey/gcdkeylinux/gcdkeyserverlinux ; $(MAKE) clean
	cd $(GOA)/ghttp/ghttpc/ghttplinux/ ; $(MAKE) clean
	cd $(GOA)/GP/gptestc/gplinux ; $(MAKE) clean
	cd $(GOA)/gstats/statstest/gstatslinux ; $(MAKE) clean
	cd $(GOA)/gstats/persisttest/persistlinux ; $(MAKE) clean
	cd $(GOA)/gt2/gt2testc/gt2linux ; $(MAKE) clean
	cd $(GOA)/natneg/simpletest/natneglinux ; $(MAKE) clean
	cd $(GOA)/Peer/peerc/peerlinux ; $(MAKE) clean
	cd $(GOA)/pt/pttestc/ptlinux ; $(MAKE) clean
	cd $(GOA)/qr2/qr2csample/qr2linux ; $(MAKE) clean
	cd $(GOA)/serverbrowsing/sbctest/sblinux ; $(MAKE) clean
#	cd $(GOA)/Voice/voicetest/voicetestlinux ; $(MAKE) clean
#	cd $(GOA)/Voice/voicebench/voicebenchlinux ; $(MAKE) clean
