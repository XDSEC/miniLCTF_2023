#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

unsigned char unk_51B0[] = {
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0xff, 0,
//flag
121, 111, 117, 95, 115, 104, 111, 117, 108, 100, 95, 112, 97, 116, 99, 104, 95, 116, 104, 105, 115, 95, 102, 108, 97, 103
};
//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//unsigned char* flag[] = {"miniLctf{QwQ_s0oOo_simple_LlVm_P4s5}"};
//miniLctf{you_should_patch_this_flag}
int l = 0;

namespace{
    struct YunZh1Jun : public FunctionPass{
        static char ID;
        YunZh1Jun() : FunctionPass(ID){}
        //void vm(); func319
        //bool check(); func629

		bool func0();
		bool func1();
		bool func2();
		bool func3();
		bool func4();
		bool func5();
		bool func6();
		bool func7();
		bool func8();
		bool func9();
		bool func10();
		bool func11();
		bool func12();
		bool func13();
		bool func14();
		bool func15();
		bool func16();
		bool func17();
		bool func18();
		bool func19();
		bool func20();
		bool func21();
		bool func22();
		bool func23();
		bool func24();
		bool func25();
		bool func26();
		bool func27();
		bool func28();
		bool func29();
		bool func30();
		bool func31();
		bool func32();
		bool func33();
		bool func34();
		bool func35();
		bool func36();
		bool func37();
		bool func38();
		bool func39();
		bool func40();
		bool func41();
		bool func42();
		bool func43();
		bool func44();
		bool func45();
		bool func46();
		bool func47();
		bool func48();
		bool func49();
		bool func50();
		bool func51();
		bool func52();
		bool func53();
		bool func54();
		bool func55();
		bool func56();
		bool func57();
		bool func58();
		bool func59();
		bool func60();
		bool func61();
		bool func62();
		bool func63();
		bool func64();
		bool func65();
		bool func66();
		bool func67();
		bool func68();
		bool func69();
		bool func70();
		bool func71();
		bool func72();
		bool func73();
		bool func74();
		bool func75();
		bool func76();
		bool func77();
		bool func78();
		bool func79();
		bool func80();
		bool func81();
		bool func82();
		bool func83();
		bool func84();
		bool func85();
		bool func86();
		bool func87();
		bool func88();
		bool func89();
		bool func90();
		bool func91();
		bool func92();
		bool func93();
		bool func94();
		bool func95();
		bool func96();
		bool func97();
		bool func98();
		bool func99();
		bool func100();
		bool func101();
		bool func102();
		bool func103();
		bool func104();
		bool func105();
		bool func106();
		bool func107();
		bool func108();
		bool func109();
		bool func110();
		bool func111();
		bool func112();
		bool func113();
		bool func114();
		bool func115();
		bool func116();
		bool func117();
		bool func118();
		bool func119();
		bool func120();
		bool func121();
		bool func122();
		bool func123();
		bool func124();
		bool func125();
		bool func126();
		bool func127();
		bool func128();
		bool func129();
		bool func130();
		bool func131();
		bool func132();
		bool func133();
		bool func134();
		bool func135();
		bool func136();
		bool func137();
		bool func138();
		bool func139();
		bool func140();
		bool func141();
		bool func142();
		bool func143();
		bool func144();
		bool func145();
		bool func146();
		bool func147();
		bool func148();
		bool func149();
		bool func150();
		bool func151();
		bool func152();
		bool func153();
		bool func154();
		bool func155();
		bool func156();
		bool func157();
		bool func158();
		bool func159();
		bool func160();
		bool func161();
		bool func162();
		bool func163();
		bool func164();
		bool func165();
		bool func166();
		bool func167();
		bool func168();
		bool func169();
		bool func170();
		bool func171();
		bool func172();
		bool func173();
		bool func174();
		bool func175();
		bool func176();
		bool func177();
		bool func178();
		bool func179();
		bool func180();
		bool func181();
		bool func182();
		bool func183();
		bool func184();
		bool func185();
		bool func186();
		bool func187();
		bool func188();
		bool func189();
		bool func190();
		bool func191();
		bool func192();
		bool func193();
		bool func194();
		bool func195();
		bool func196();
		bool func197();
		bool func198();
		bool func199();
		bool func200();
		bool func201();
		bool func202();
		bool func203();
		bool func204();
		bool func205();
		bool func206();
		bool func207();
		bool func208();
		bool func209();
		bool func210();
		bool func211();
		bool func212();
		bool func213();
		bool func214();
		bool func215();
		bool func216();
		bool func217();
		bool func218();
		bool func219();
		bool func220();
		bool func221();
		bool func222();
		bool func223();
		bool func224();
		bool func225();
		bool func226();
		bool func227();
		bool func228();
		bool func229();
		bool func230();
		bool func231();
		bool func232();
		bool func233();
		bool func234();
		bool func235();
		bool func236();
		bool func237();
		bool func238();
		bool func239();
		bool func240();
		bool func241();
		bool func242();
		bool func243();
		bool func244();
		bool func245();
		bool func246();
		bool func247();
		bool func248();
		bool func249();
		bool func250();
		bool func251();
		bool func252();
		bool func253();
		bool func254();
		bool func255();
		bool func256();
		bool func257();
		bool func258();
		bool func259();
		bool func260();
		bool func261();
		bool func262();
		bool func263();
		bool func264();
		bool func265();
		bool func266();
		bool func267();
		bool func268();
		bool func269();
		bool func270();
		bool func271();
		bool func272();
		bool func273();
		bool func274();
		bool func275();
		bool func276();
		bool func277();
		bool func278();
		bool func279();
		bool func280();
		bool func281();
		bool func282();
		bool func283();
		bool func284();
		bool func285();
		bool func286();
		bool func287();
		bool func288();
		bool func289();
		bool func290();
		bool func291();
		bool func292();
		bool func293();
		bool func294();
		bool func295();
		bool func296();
		bool func297();
		bool func298();
		bool func299();
		bool func300();
		bool func301();
		bool func302();
		bool func303();
		bool func304();
		bool func305();
		bool func306();
		bool func307();
		bool func308();
		bool func309();
		bool func310();
		bool func311();
		bool func312();
		bool func313();
		bool func314();
		bool func315();
		bool func316();
		bool func317();
		bool func318();
		void func319();
		bool func320();
		bool func321();
		bool func322();
		bool func323();
		bool func324();
		bool func325();
		bool func326();
		bool func327();
		bool func328();
		bool func329();
		bool func330();
		bool func331();
		bool func332();
		bool func333();
		bool func334();
		bool func335();
		bool func336();
		bool func337();
		bool func338();
		bool func339();
		bool func340();
		bool func341();
		bool func342();
		bool func343();
		bool func344();
		bool func345();
		bool func346();
		bool func347();
		bool func348();
		bool func349();
		bool func350();
		bool func351();
		bool func352();
		bool func353();
		bool func354();
		bool func355();
		bool func356();
		bool func357();
		bool func358();
		bool func359();
		bool func360();
		bool func361();
		bool func362();
		bool func363();
		bool func364();
		bool func365();
		bool func366();
		bool func367();
		bool func368();
		bool func369();
		bool func370();
		bool func371();
		bool func372();
		bool func373();
		bool func374();
		bool func375();
		bool func376();
		bool func377();
		bool func378();
		bool func379();
		bool func380();
		bool func381();
		bool func382();
		bool func383();
		bool func384();
		bool func385();
		bool func386();
		bool func387();
		bool func388();
		bool func389();
		bool func390();
		bool func391();
		bool func392();
		bool func393();
		bool func394();
		bool func395();
		bool func396();
		bool func397();
		bool func398();
		bool func399();
		bool func400();
		bool func401();
		bool func402();
		bool func403();
		bool func404();
		bool func405();
		bool func406();
		bool func407();
		bool func408();
		bool func409();
		bool func410();
		bool func411();
		bool func412();
		bool func413();
		bool func414();
		bool func415();
		bool func416();
		bool func417();
		bool func418();
		bool func419();
		bool func420();
		bool func421();
		bool func422();
		bool func423();
		bool func424();
		bool func425();
		bool func426();
		bool func427();
		bool func428();
		bool func429();
		bool func430();
		bool func431();
		bool func432();
		bool func433();
		bool func434();
		bool func435();
		bool func436();
		bool func437();
		bool func438();
		bool func439();
		bool func440();
		bool func441();
		bool func442();
		bool func443();
		bool func444();
		bool func445();
		bool func446();
		bool func447();
		bool func448();
		bool func449();
		bool func450();
		bool func451();
		bool func452();
		bool func453();
		bool func454();
		bool func455();
		bool func456();
		bool func457();
		bool func458();
		bool func459();
		bool func460();
		bool func461();
		bool func462();
		bool func463();
		bool func464();
		bool func465();
		bool func466();
		bool func467();
		bool func468();
		bool func469();
		bool func470();
		bool func471();
		bool func472();
		bool func473();
		bool func474();
		bool func475();
		bool func476();
		bool func477();
		bool func478();
		bool func479();
		bool func480();
		bool func481();
		bool func482();
		bool func483();
		bool func484();
		bool func485();
		bool func486();
		bool func487();
		bool func488();
		bool func489();
		bool func490();
		bool func491();
		bool func492();
		bool func493();
		bool func494();
		bool func495();
		bool func496();
		bool func497();
		bool func498();
		bool func499();
		bool func500();
		bool func501();
		bool func502();
		bool func503();
		bool func504();
		bool func505();
		bool func506();
		bool func507();
		bool func508();
		bool func509();
		bool func510();
		bool func511();
		bool func512();
		bool func513();
		bool func514();
		bool func515();
		bool func516();
		bool func517();
		bool func518();
		bool func519();
		bool func520();
		bool func521();
		bool func522();
		bool func523();
		bool func524();
		bool func525();
		bool func526();
		bool func527();
		bool func528();
		bool func529();
		bool func530();
		bool func531();
		bool func532();
		bool func533();
		bool func534();
		bool func535();
		bool func536();
		bool func537();
		bool func538();
		bool func539();
		bool func540();
		bool func541();
		bool func542();
		bool func543();
		bool func544();
		bool func545();
		bool func546();
		bool func547();
		bool func548();
		bool func549();
		bool func550();
		bool func551();
		bool func552();
		bool func553();
		bool func554();
		bool func555();
		bool func556();
		bool func557();
		bool func558();
		bool func559();
		bool func560();
		bool func561();
		bool func562();
		bool func563();
		bool func564();
		bool func565();
		bool func566();
		bool func567();
		bool func568();
		bool func569();
		bool func570();
		bool func571();
		bool func572();
		bool func573();
		bool func574();
		bool func575();
		bool func576();
		bool func577();
		bool func578();
		bool func579();
		bool func580();
		bool func581();
		bool func582();
		bool func583();
		bool func584();
		bool func585();
		bool func586();
		bool func587();
		bool func588();
		bool func589();
		bool func590();
		bool func591();
		bool func592();
		bool func593();
		bool func594();
		bool func595();
		bool func596();
		bool func597();
		bool func598();
		bool func599();
		bool func600();
		bool func601();
		bool func602();
		bool func603();
		bool func604();
		bool func605();
		bool func606();
		bool func607();
		bool func608();
		bool func609();
		bool func610();
		bool func611();
		bool func612();
		bool func613();
		bool func614();
		bool func615();
		bool func616();
		bool func617();
		bool func618();
		bool func619();
		bool func620();
		bool func621();
		bool func622();
		bool func623();
		bool func624();
		bool func625();
		bool func626();
		bool func627();
		bool func628();
		bool func629();
		bool func630();
		bool func631();
		bool func632();
		bool func633();
		bool func634();
		bool func635();
		bool func636();
		bool func637();
		bool func638();
		bool func639();
		bool func640();
		bool func641();
		bool func642();
		bool func643();
		bool func644();
		bool func645();
		bool func646();
		bool func647();
		bool func648();
		bool func649();
		bool func650();
		bool func651();
		bool func652();
		bool func653();
		bool func654();
		bool func655();
		bool func656();
		bool func657();
		bool func658();
		bool func659();
		bool func660();
		bool func661();
		bool func662();
		bool func663();
		bool func664();
		bool func665();
		bool func666();
		bool func667();
		bool func668();
		bool func669();
		bool func670();
		bool func671();
		bool func672();
		bool func673();
		bool func674();
		bool func675();
		bool func676();
		bool func677();
		bool func678();
		bool func679();
		bool func680();
		bool func681();
		bool func682();
		bool func683();
		bool func684();
		bool func685();
		bool func686();
		bool func687();
		bool func688();
		bool func689();
		bool func690();
		bool func691();
		bool func692();
		bool func693();
		bool func694();
		bool func695();
		bool func696();
		bool func697();
		bool func698();
		bool func699();
		bool func700();
		bool func701();
		bool func702();
		bool func703();
		bool func704();
		bool func705();
		bool func706();
		bool func707();
		bool func708();
		bool func709();
		bool func710();
		bool func711();
		bool func712();
		bool func713();
		bool func714();
		bool func715();
		bool func716();
		bool func717();
		bool func718();
		bool func719();
		bool func720();
		bool func721();
		bool func722();
		bool func723();
		bool func724();
		bool func725();
		bool func726();
		bool func727();
		bool func728();
		bool func729();
		bool func730();
		bool func731();
		bool func732();
		bool func733();
		bool func734();
		bool func735();
		bool func736();
		bool func737();
		bool func738();
		bool func739();
		bool func740();
		bool func741();
		bool func742();
		bool func743();
		bool func744();
		bool func745();
		bool func746();
		bool func747();
		bool func748();
		bool func749();
		bool func750();
		bool func751();
		bool func752();
		bool func753();
		bool func754();
		bool func755();
		bool func756();
		bool func757();
		bool func758();
		bool func759();
		bool func760();
		bool func761();
		bool func762();
		bool func763();
		bool func764();
		bool func765();
		bool func766();
		bool func767();
		bool func768();
		bool func769();
		bool func770();
		bool func771();
		bool func772();
		bool func773();
		bool func774();
		bool func775();
		bool func776();
		bool func777();
		bool func778();
		bool func779();
		bool func780();
		bool func781();
		bool func782();
		bool func783();
		bool func784();
		bool func785();
		bool func786();
		bool func787();
		bool func788();
		bool func789();
		bool func790();
		bool func791();
		bool func792();
		bool func793();
		bool func794();
		bool func795();
		bool func796();
		bool func797();
		bool func798();
		bool func799();
		bool func800();
		bool func801();
		bool func802();
		bool func803();
		bool func804();
		bool func805();
		bool func806();
		bool func807();
		bool func808();
		bool func809();
		bool func810();
		bool func811();
		bool func812();
		bool func813();
		bool func814();
		bool func815();
		bool func816();
		bool func817();
		bool func818();
		bool func819();
		bool func820();
		bool func821();
		bool func822();
		bool func823();
		bool func824();
		bool func825();
		bool func826();
		bool func827();
		bool func828();
		bool func829();
		bool func830();
		bool func831();
		bool func832();
		bool func833();
		bool func834();
		bool func835();
		bool func836();
		bool func837();
		bool func838();
		bool func839();
		bool func840();
		bool func841();
		bool func842();
		bool func843();
		bool func844();
		bool func845();
		bool func846();
		bool func847();
		bool func848();
		bool func849();
		bool func850();
		bool func851();
		bool func852();
		bool func853();
		bool func854();
		bool func855();
		bool func856();
		bool func857();
		bool func858();
		bool func859();
		bool func860();
		bool func861();
		bool func862();
		bool func863();
		bool func864();
		bool func865();
		bool func866();
		bool func867();
		bool func868();
		bool func869();
		bool func870();
		bool func871();
		bool func872();
		bool func873();
		bool func874();
		bool func875();
		bool func876();
		bool func877();
		bool func878();
		bool func879();
		bool func880();
		bool func881();
		bool func882();
		bool func883();
		bool func884();
		bool func885();
		bool func886();
		bool func887();
		bool func888();
		bool func889();
		bool func890();
		bool func891();
		bool func892();
		bool func893();
		bool func894();
		bool func895();
		bool func896();
		bool func897();
		bool func898();
		bool func899();
		bool func900();
		bool func901();
		bool func902();
		bool func903();
		bool func904();
		bool func905();
		bool func906();
		bool func907();
		bool func908();
		bool func909();
		bool func910();
		bool func911();
		bool func912();
		bool func913();
		bool func914();
		bool func915();
		bool func916();
		bool func917();
		bool func918();
		bool func919();
		bool func920();
		bool func921();
		bool func922();
		bool func923();
		bool func924();
		bool func925();
		bool func926();
		bool func927();
		bool func928();
		bool func929();
		bool func930();
		bool func931();
		bool func932();
		bool func933();
		bool func934();
		bool func935();
		bool func936();
		bool func937();
		bool func938();
		bool func939();
		bool func940();
		bool func941();
		bool func942();
		bool func943();
		bool func944();
		bool func945();
		bool func946();
		bool func947();
		bool func948();
		bool func949();
		bool func950();
		bool func951();
		bool func952();
		bool func953();
		bool func954();
		bool func955();
		bool func956();
		bool func957();
		bool func958();
		bool func959();
		bool func960();
		bool func961();
		bool func962();
		bool func963();
		bool func964();
		bool func965();
		bool func966();
		bool func967();
		bool func968();
		bool func969();
		bool func970();
		bool func971();
		bool func972();
		bool func973();
		bool func974();
		bool func975();
		bool func976();
		bool func977();
		bool func978();
		bool func979();
		bool func980();
		bool func981();
		bool func982();
		bool func983();
		bool func984();
		bool func985();
		bool func986();
		bool func987();
		bool func988();
		bool func989();
		bool func990();
		bool func991();
		bool func992();
		bool func993();
		bool func994();
		bool func995();
		bool func996();
		bool func997();
		bool func998();
		bool func999();

        bool runOnFunction(Function &F) override{
			func0();
			func1();
			func2();
			func3();
			func4();
			func5();
			func6();
			func7();
			func8();
			func9();
			func10();
			func11();
			func12();
			func13();
			func14();
			func15();
			func16();
			func17();
			func18();
			func19();
			func20();
			func21();
			func22();
			func23();
			func24();
			func25();
			func26();
			func27();
			func28();
			func29();
			func30();
			func31();
			func32();
			func33();
			func34();
			func35();
			func36();
			func37();
			func38();
			func39();
			func40();
			func41();
			func42();
			func43();
			func44();
			func45();
			func46();
			func47();
			func48();
			func49();
			func50();
			func51();
			func52();
			func53();
			func54();
			func55();
			func56();
			func57();
			func58();
			func59();
			func60();
			func61();
			func62();
			func63();
			func64();
			func65();
			func66();
			func67();
			func68();
			func69();
			func70();
			func71();
			func72();
			func73();
			func74();
			func75();
			func76();
			func77();
			func78();
			func79();
			func80();
			func81();
			func82();
			func83();
			func84();
			func85();
			func86();
			func87();
			func88();
			func89();
			func90();
			func91();
			func92();
			func93();
			func94();
			func95();
			func96();
			func97();
			func98();
			func99();
			func100();
			func101();
			func102();
			func103();
			func104();
			func105();
			func106();
			func107();
			func108();
			func109();
			func110();
			func111();
			func112();
			func113();
			func114();
			func115();
			func116();
			func117();
			func118();
			func119();
			func120();
			func121();
			func122();
			func123();
			func124();
			func125();
			func126();
			func127();
			func128();
			func129();
			func130();
			func131();
			func132();
			func133();
			func134();
			func135();
			func136();
			func137();
			func138();
			func139();
			func140();
			func141();
			func142();
			func143();
			func144();
			func145();
			func146();
			func147();
			func148();
			func149();
			func150();
			func151();
			func152();
			func153();
			func154();
			func155();
			func156();
			func157();
			func158();
			func159();
			func160();
			func161();
			func162();
			func163();
			func164();
			func165();
			func166();
			func167();
			func168();
			func169();
			func170();
			func171();
			func172();
			func173();
			func174();
			func175();
			func176();
			func177();
			func178();
			func179();
			func180();
			func181();
			func182();
			func183();
			func184();
			func185();
			func186();
			func187();
			func188();
			func189();
			func190();
			func191();
			func192();
			func193();
			func194();
			func195();
			func196();
			func197();
			func198();
			func199();
			func200();
			func201();
			func202();
			func203();
			func204();
			func205();
			func206();
			func207();
			func208();
			func209();
			func210();
			func211();
			func212();
			func213();
			func214();
			func215();
			func216();
			func217();
			func218();
			func219();
			func220();
			func221();
			func222();
			func223();
			func224();
			func225();
			func226();
			func227();
			func228();
			func229();
			func230();
			func231();
			func232();
			func233();
			func234();
			func235();
			func236();
			func237();
			func238();
			func239();
			func240();
			func241();
			func242();
			func243();
			func244();
			func245();
			func246();
			func247();
			func248();
			func249();
			func250();
			func251();
			func252();
			func253();
			func254();
			func255();
			func256();
			func257();
			func258();
			func259();
			func260();
			func261();
			func262();
			func263();
			func264();
			func265();
			func266();
			func267();
			func268();
			func269();
			func270();
			func271();
			func272();
			func273();
			func274();
			func275();
			func276();
			func277();
			func278();
			func279();
			func280();
			func281();
			func282();
			func283();
			func284();
			func285();
			func286();
			func287();
			func288();
			func289();
			func290();
			func291();
			func292();
			func293();
			func294();
			func295();
			func296();
			func297();
			func298();
			func299();
			func300();
			func301();
			func302();
			func303();
			func304();
			func305();
			func306();
			func307();
			func308();
			func309();
			func310();
			func311();
			func312();
			func313();
			func314();
			func315();
			func316();
			func317();
			func318();
			//func319();
			func320();
			func321();
			func322();
			func323();
			func324();
			func325();
			func326();
			func327();
			func328();
			func329();
			func330();
			func331();
			func332();
			func333();
			func334();
			func335();
			func336();
			func337();
			func338();
			func339();
			func340();
			func341();
			func342();
			func343();
			func344();
			func345();
			func346();
			func347();
			func348();
			func349();
			func350();
			func351();
			func352();
			func353();
			func354();
			func355();
			func356();
			func357();
			func358();
			func359();
			func360();
			func361();
			func362();
			func363();
			func364();
			func365();
			func366();
			func367();
			func368();
			func369();
			func370();
			func371();
			func372();
			func373();
			func374();
			func375();
			func376();
			func377();
			func378();
			func379();
			func380();
			func381();
			func382();
			func383();
			func384();
			func385();
			func386();
			func387();
			func388();
			func389();
			func390();
			func391();
			func392();
			func393();
			func394();
			func395();
			func396();
			func397();
			func398();
			func399();
			func400();
			func401();
			func402();
			func403();
			func404();
			func405();
			func406();
			func407();
			func408();
			func409();
			func410();
			func411();
			func412();
			func413();
			func414();
			func415();
			func416();
			func417();
			func418();
			func419();
			func420();
			func421();
			func422();
			func423();
			func424();
			func425();
			func426();
			func427();
			func428();
			func429();
			func430();
			func431();
			func432();
			func433();
			func434();
			func435();
			func436();
			func437();
			func438();
			func439();
			func440();
			func441();
			func442();
			func443();
			func444();
			func445();
			func446();
			func447();
			func448();
			func449();
			func450();
			func451();
			func452();
			func453();
			func454();
			func455();
			func456();
			func457();
			func458();
			func459();
			func460();
			func461();
			func462();
			func463();
			func464();
			func465();
			func466();
			func467();
			func468();
			func469();
			func470();
			func471();
			func472();
			func473();
			func474();
			func475();
			func476();
			func477();
			func478();
			func479();
			func480();
			func481();
			func482();
			func483();
			func484();
			func485();
			func486();
			func487();
			func488();
			func489();
			func490();
			func491();
			func492();
			func493();
			func494();
			func495();
			func496();
			func497();
			func498();
			func499();
			func500();
			func501();
			func502();
			func503();
			func504();
			func505();
			func506();
			func507();
			func508();
			func509();
			func510();
			func511();
			func512();
			func513();
			func514();
			func515();
			func516();
			func517();
			func518();
			func519();
			func520();
			func521();
			func522();
			func523();
			func524();
			func525();
			func526();
			func527();
			func528();
			func529();
			func530();
			func531();
			func532();
			func533();
			func534();
			func535();
			func536();
			func537();
			func538();
			func539();
			func540();
			func541();
			func542();
			func543();
			func544();
			func545();
			func546();
			func547();
			func548();
			func549();
			func550();
			func551();
			func552();
			func553();
			func554();
			func555();
			func556();
			func557();
			func558();
			func559();
			func560();
			func561();
			func562();
			func563();
			func564();
			func565();
			func566();
			func567();
			func568();
			func569();
			func570();
			func571();
			func572();
			func573();
			func574();
			func575();
			func576();
			func577();
			func578();
			func579();
			func580();
			func581();
			func582();
			func583();
			func584();
			func585();
			func586();
			func587();
			func588();
			func589();
			func590();
			func591();
			func592();
			func593();
			func594();
			func595();
			func596();
			func597();
			func598();
			func599();
			func600();
			func601();
			func602();
			func603();
			func604();
			func605();
			func606();
			func607();
			func608();
			func609();
			func610();
			func611();
			func612();
			func613();
			func614();
			func615();
			func616();
			func617();
			func618();
			func619();
			func620();
			func621();
			func622();
			func623();
			func624();
			func625();
			func626();
			func627();
			func628();
			//func629();
			func630();
			func631();
			func632();
			func633();
			func634();
			func635();
			func636();
			func637();
			func638();
			func639();
			func640();
			func641();
			func642();
			func643();
			func644();
			func645();
			func646();
			func647();
			func648();
			func649();
			func650();
			func651();
			func652();
			func653();
			func654();
			func655();
			func656();
			func657();
			func658();
			func659();
			func660();
			func661();
			func662();
			func663();
			func664();
			func665();
			func666();
			func667();
			func668();
			func669();
			func670();
			func671();
			func672();
			func673();
			func674();
			func675();
			func676();
			func677();
			func678();
			func679();
			func680();
			func681();
			func682();
			func683();
			func684();
			func685();
			func686();
			func687();
			func688();
			func689();
			func690();
			func691();
			func692();
			func693();
			func694();
			func695();
			func696();
			func697();
			func698();
			func699();
			func700();
			func701();
			func702();
			func703();
			func704();
			func705();
			func706();
			func707();
			func708();
			func709();
			func710();
			func711();
			func712();
			func713();
			func714();
			func715();
			func716();
			func717();
			func718();
			func719();
			func720();
			func721();
			func722();
			func723();
			func724();
			func725();
			func726();
			func727();
			func728();
			func729();
			func730();
			func731();
			func732();
			func733();
			func734();
			func735();
			func736();
			func737();
			func738();
			func739();
			func740();
			func741();
			func742();
			func743();
			func744();
			func745();
			func746();
			func747();
			func748();
			func749();
			func750();
			func751();
			func752();
			func753();
			func754();
			func755();
			func756();
			func757();
			func758();
			func759();
			func760();
			func761();
			func762();
			func763();
			func764();
			func765();
			func766();
			func767();
			func768();
			func769();
			func770();
			func771();
			func772();
			func773();
			func774();
			func775();
			func776();
			func777();
			func778();
			func779();
			func780();
			func781();
			func782();
			func783();
			func784();
			func785();
			func786();
			func787();
			func788();
			func789();
			func790();
			func791();
			func792();
			func793();
			func794();
			func795();
			func796();
			func797();
			func798();
			func799();
			func800();
			func801();
			func802();
			func803();
			func804();
			func805();
			func806();
			func807();
			func808();
			func809();
			func810();
			func811();
			func812();
			func813();
			func814();
			func815();
			func816();
			func817();
			func818();
			func819();
			func820();
			func821();
			func822();
			func823();
			func824();
			func825();
			func826();
			func827();
			func828();
			func829();
			func830();
			func831();
			func832();
			func833();
			func834();
			func835();
			func836();
			func837();
			func838();
			func839();
			func840();
			func841();
			func842();
			func843();
			func844();
			func845();
			func846();
			func847();
			func848();
			func849();
			func850();
			func851();
			func852();
			func853();
			func854();
			func855();
			func856();
			func857();
			func858();
			func859();
			func860();
			func861();
			func862();
			func863();
			func864();
			func865();
			func866();
			func867();
			func868();
			func869();
			func870();
			func871();
			func872();
			func873();
			func874();
			func875();
			func876();
			func877();
			func878();
			func879();
			func880();
			func881();
			func882();
			func883();
			func884();
			func885();
			func886();
			func887();
			func888();
			func889();
			func890();
			func891();
			func892();
			func893();
			func894();
			func895();
			func896();
			func897();
			func898();
			func899();
			func900();
			func901();
			func902();
			func903();
			func904();
			func905();
			func906();
			func907();
			func908();
			func909();
			func910();
			func911();
			func912();
			func913();
			func914();
			func915();
			func916();
			func917();
			func918();
			func919();
			func920();
			func921();
			func922();
			func923();
			func924();
			func925();
			func926();
			func927();
			func928();
			func929();
			func930();
			func931();
			func932();
			func933();
			func934();
			func935();
			func936();
			func937();
			func938();
			func939();
			func940();
			func941();
			func942();
			func943();
			func944();
			func945();
			func946();
			func947();
			func948();
			func949();
			func950();
			func951();
			func952();
			func953();
			func954();
			func955();
			func956();
			func957();
			func958();
			func959();
			func960();
			func961();
			func962();
			func963();
			func964();
			func965();
			func966();
			func967();
			func968();
			func969();
			func970();
			func971();
			func972();
			func973();
			func974();
			func975();
			func976();
			func977();
			func978();
			func979();
			func980();
			func981();
			func982();
			func983();
			func984();
			func985();
			func986();
			func987();
			func988();
			func989();
			func990();
			func991();
			func992();
			func993();
			func994();
			func995();
			func996();
			func997();
			func998();
			func999();

            //outs() << "Function: " << F.getName() ;

            for(int i=0;i<15;i++)
                unk_51B0[i+1]=F.getName().str().c_str()[i];

            unsigned char right[]={75, 112, 126, 113, 109, 56, 57, 93, 118, 119, 62, 109, 57, 127, 118, 107, 126, 124, 109, 57, 116, 112, 119, 112, 85, 122, 109, 127, 98, 100, 103,0};
            unsigned char wrong[]={72, 110, 72, 53, 57, 105, 117, 99, 57, 109, 107, 96, 57, 120, 126, 120, 112, 119, 103, 0};

            func319();
            l++;
            if(l == 26){
                if(func629()){
                    for(int i=0;i<31;i++){
                        right[i] ^= 0x19;
                        outs() << *(right + i);
                    }
                    outs() << "\n";
                }
                else{
                    for(int i=0;i<19;i++){
                        wrong[i] ^= 0x19;
                        outs() << *(wrong + i);
                    }
                    outs() << "\n";
                }
            }

            return false;
        }
    };
}

bool YunZh1Jun::func0(){return true;}
bool YunZh1Jun::func1(){return true;}
bool YunZh1Jun::func2(){return true;}
bool YunZh1Jun::func3(){return true;}
bool YunZh1Jun::func4(){return true;}
bool YunZh1Jun::func5(){return true;}
bool YunZh1Jun::func6(){return true;}
bool YunZh1Jun::func7(){return true;}
bool YunZh1Jun::func8(){return true;}
bool YunZh1Jun::func9(){return true;}
bool YunZh1Jun::func10(){return true;}
bool YunZh1Jun::func11(){return true;}
bool YunZh1Jun::func12(){return true;}
bool YunZh1Jun::func13(){return true;}
bool YunZh1Jun::func14(){return true;}
bool YunZh1Jun::func15(){return true;}
bool YunZh1Jun::func16(){return true;}
bool YunZh1Jun::func17(){return true;}
bool YunZh1Jun::func18(){return true;}
bool YunZh1Jun::func19(){return true;}
bool YunZh1Jun::func20(){return true;}
bool YunZh1Jun::func21(){return true;}
bool YunZh1Jun::func22(){return true;}
bool YunZh1Jun::func23(){return true;}
bool YunZh1Jun::func24(){return true;}
bool YunZh1Jun::func25(){return true;}
bool YunZh1Jun::func26(){return true;}
bool YunZh1Jun::func27(){return true;}
bool YunZh1Jun::func28(){return true;}
bool YunZh1Jun::func29(){return true;}
bool YunZh1Jun::func30(){return true;}
bool YunZh1Jun::func31(){return true;}
bool YunZh1Jun::func32(){return true;}
bool YunZh1Jun::func33(){return true;}
bool YunZh1Jun::func34(){return true;}
bool YunZh1Jun::func35(){return true;}
bool YunZh1Jun::func36(){return true;}
bool YunZh1Jun::func37(){return true;}
bool YunZh1Jun::func38(){return true;}
bool YunZh1Jun::func39(){return true;}
bool YunZh1Jun::func40(){return true;}
bool YunZh1Jun::func41(){return true;}
bool YunZh1Jun::func42(){return true;}
bool YunZh1Jun::func43(){return true;}
bool YunZh1Jun::func44(){return true;}
bool YunZh1Jun::func45(){return true;}
bool YunZh1Jun::func46(){return true;}
bool YunZh1Jun::func47(){return true;}
bool YunZh1Jun::func48(){return true;}
bool YunZh1Jun::func49(){return true;}
bool YunZh1Jun::func50(){return true;}
bool YunZh1Jun::func51(){return true;}
bool YunZh1Jun::func52(){return true;}
bool YunZh1Jun::func53(){return true;}
bool YunZh1Jun::func54(){return true;}
bool YunZh1Jun::func55(){return true;}
bool YunZh1Jun::func56(){return true;}
bool YunZh1Jun::func57(){return true;}
bool YunZh1Jun::func58(){return true;}
bool YunZh1Jun::func59(){return true;}
bool YunZh1Jun::func60(){return true;}
bool YunZh1Jun::func61(){return true;}
bool YunZh1Jun::func62(){return true;}
bool YunZh1Jun::func63(){return true;}
bool YunZh1Jun::func64(){return true;}
bool YunZh1Jun::func65(){return true;}
bool YunZh1Jun::func66(){return true;}
bool YunZh1Jun::func67(){return true;}
bool YunZh1Jun::func68(){return true;}
bool YunZh1Jun::func69(){return true;}
bool YunZh1Jun::func70(){return true;}
bool YunZh1Jun::func71(){return true;}
bool YunZh1Jun::func72(){return true;}
bool YunZh1Jun::func73(){return true;}
bool YunZh1Jun::func74(){return true;}
bool YunZh1Jun::func75(){return true;}
bool YunZh1Jun::func76(){return true;}
bool YunZh1Jun::func77(){return true;}
bool YunZh1Jun::func78(){return true;}
bool YunZh1Jun::func79(){return true;}
bool YunZh1Jun::func80(){return true;}
bool YunZh1Jun::func81(){return true;}
bool YunZh1Jun::func82(){return true;}
bool YunZh1Jun::func83(){return true;}
bool YunZh1Jun::func84(){return true;}
bool YunZh1Jun::func85(){return true;}
bool YunZh1Jun::func86(){return true;}
bool YunZh1Jun::func87(){return true;}
bool YunZh1Jun::func88(){return true;}
bool YunZh1Jun::func89(){return true;}
bool YunZh1Jun::func90(){return true;}
bool YunZh1Jun::func91(){return true;}
bool YunZh1Jun::func92(){return true;}
bool YunZh1Jun::func93(){return true;}
bool YunZh1Jun::func94(){return true;}
bool YunZh1Jun::func95(){return true;}
bool YunZh1Jun::func96(){return true;}
bool YunZh1Jun::func97(){return true;}
bool YunZh1Jun::func98(){return true;}
bool YunZh1Jun::func99(){return true;}
bool YunZh1Jun::func100(){return true;}
bool YunZh1Jun::func101(){return true;}
bool YunZh1Jun::func102(){return true;}
bool YunZh1Jun::func103(){return true;}
bool YunZh1Jun::func104(){return true;}
bool YunZh1Jun::func105(){return true;}
bool YunZh1Jun::func106(){return true;}
bool YunZh1Jun::func107(){return true;}
bool YunZh1Jun::func108(){return true;}
bool YunZh1Jun::func109(){return true;}
bool YunZh1Jun::func110(){return true;}
bool YunZh1Jun::func111(){return true;}
bool YunZh1Jun::func112(){return true;}
bool YunZh1Jun::func113(){return true;}
bool YunZh1Jun::func114(){return true;}
bool YunZh1Jun::func115(){return true;}
bool YunZh1Jun::func116(){return true;}
bool YunZh1Jun::func117(){return true;}
bool YunZh1Jun::func118(){return true;}
bool YunZh1Jun::func119(){return true;}
bool YunZh1Jun::func120(){return true;}
bool YunZh1Jun::func121(){return true;}
bool YunZh1Jun::func122(){return true;}
bool YunZh1Jun::func123(){return true;}
bool YunZh1Jun::func124(){return true;}
bool YunZh1Jun::func125(){return true;}
bool YunZh1Jun::func126(){return true;}
bool YunZh1Jun::func127(){return true;}
bool YunZh1Jun::func128(){return true;}
bool YunZh1Jun::func129(){return true;}
bool YunZh1Jun::func130(){return true;}
bool YunZh1Jun::func131(){return true;}
bool YunZh1Jun::func132(){return true;}
bool YunZh1Jun::func133(){return true;}
bool YunZh1Jun::func134(){return true;}
bool YunZh1Jun::func135(){return true;}
bool YunZh1Jun::func136(){return true;}
bool YunZh1Jun::func137(){return true;}
bool YunZh1Jun::func138(){return true;}
bool YunZh1Jun::func139(){return true;}
bool YunZh1Jun::func140(){return true;}
bool YunZh1Jun::func141(){return true;}
bool YunZh1Jun::func142(){return true;}
bool YunZh1Jun::func143(){return true;}
bool YunZh1Jun::func144(){return true;}
bool YunZh1Jun::func145(){return true;}
bool YunZh1Jun::func146(){return true;}
bool YunZh1Jun::func147(){return true;}
bool YunZh1Jun::func148(){return true;}
bool YunZh1Jun::func149(){return true;}
bool YunZh1Jun::func150(){return true;}
bool YunZh1Jun::func151(){return true;}
bool YunZh1Jun::func152(){return true;}
bool YunZh1Jun::func153(){return true;}
bool YunZh1Jun::func154(){return true;}
bool YunZh1Jun::func155(){return true;}
bool YunZh1Jun::func156(){return true;}
bool YunZh1Jun::func157(){return true;}
bool YunZh1Jun::func158(){return true;}
bool YunZh1Jun::func159(){return true;}
bool YunZh1Jun::func160(){return true;}
bool YunZh1Jun::func161(){return true;}
bool YunZh1Jun::func162(){return true;}
bool YunZh1Jun::func163(){return true;}
bool YunZh1Jun::func164(){return true;}
bool YunZh1Jun::func165(){return true;}
bool YunZh1Jun::func166(){return true;}
bool YunZh1Jun::func167(){return true;}
bool YunZh1Jun::func168(){return true;}
bool YunZh1Jun::func169(){return true;}
bool YunZh1Jun::func170(){return true;}
bool YunZh1Jun::func171(){return true;}
bool YunZh1Jun::func172(){return true;}
bool YunZh1Jun::func173(){return true;}
bool YunZh1Jun::func174(){return true;}
bool YunZh1Jun::func175(){return true;}
bool YunZh1Jun::func176(){return true;}
bool YunZh1Jun::func177(){return true;}
bool YunZh1Jun::func178(){return true;}
bool YunZh1Jun::func179(){return true;}
bool YunZh1Jun::func180(){return true;}
bool YunZh1Jun::func181(){return true;}
bool YunZh1Jun::func182(){return true;}
bool YunZh1Jun::func183(){return true;}
bool YunZh1Jun::func184(){return true;}
bool YunZh1Jun::func185(){return true;}
bool YunZh1Jun::func186(){return true;}
bool YunZh1Jun::func187(){return true;}
bool YunZh1Jun::func188(){return true;}
bool YunZh1Jun::func189(){return true;}
bool YunZh1Jun::func190(){return true;}
bool YunZh1Jun::func191(){return true;}
bool YunZh1Jun::func192(){return true;}
bool YunZh1Jun::func193(){return true;}
bool YunZh1Jun::func194(){return true;}
bool YunZh1Jun::func195(){return true;}
bool YunZh1Jun::func196(){return true;}
bool YunZh1Jun::func197(){return true;}
bool YunZh1Jun::func198(){return true;}
bool YunZh1Jun::func199(){return true;}
bool YunZh1Jun::func200(){return true;}
bool YunZh1Jun::func201(){return true;}
bool YunZh1Jun::func202(){return true;}
bool YunZh1Jun::func203(){return true;}
bool YunZh1Jun::func204(){return true;}
bool YunZh1Jun::func205(){return true;}
bool YunZh1Jun::func206(){return true;}
bool YunZh1Jun::func207(){return true;}
bool YunZh1Jun::func208(){return true;}
bool YunZh1Jun::func209(){return true;}
bool YunZh1Jun::func210(){return true;}
bool YunZh1Jun::func211(){return true;}
bool YunZh1Jun::func212(){return true;}
bool YunZh1Jun::func213(){return true;}
bool YunZh1Jun::func214(){return true;}
bool YunZh1Jun::func215(){return true;}
bool YunZh1Jun::func216(){return true;}
bool YunZh1Jun::func217(){return true;}
bool YunZh1Jun::func218(){return true;}
bool YunZh1Jun::func219(){return true;}
bool YunZh1Jun::func220(){return true;}
bool YunZh1Jun::func221(){return true;}
bool YunZh1Jun::func222(){return true;}
bool YunZh1Jun::func223(){return true;}
bool YunZh1Jun::func224(){return true;}
bool YunZh1Jun::func225(){return true;}
bool YunZh1Jun::func226(){return true;}
bool YunZh1Jun::func227(){return true;}
bool YunZh1Jun::func228(){return true;}
bool YunZh1Jun::func229(){return true;}
bool YunZh1Jun::func230(){return true;}
bool YunZh1Jun::func231(){return true;}
bool YunZh1Jun::func232(){return true;}
bool YunZh1Jun::func233(){return true;}
bool YunZh1Jun::func234(){return true;}
bool YunZh1Jun::func235(){return true;}
bool YunZh1Jun::func236(){return true;}
bool YunZh1Jun::func237(){return true;}
bool YunZh1Jun::func238(){return true;}
bool YunZh1Jun::func239(){return true;}
bool YunZh1Jun::func240(){return true;}
bool YunZh1Jun::func241(){return true;}
bool YunZh1Jun::func242(){return true;}
bool YunZh1Jun::func243(){return true;}
bool YunZh1Jun::func244(){return true;}
bool YunZh1Jun::func245(){return true;}
bool YunZh1Jun::func246(){return true;}
bool YunZh1Jun::func247(){return true;}
bool YunZh1Jun::func248(){return true;}
bool YunZh1Jun::func249(){return true;}
bool YunZh1Jun::func250(){return true;}
bool YunZh1Jun::func251(){return true;}
bool YunZh1Jun::func252(){return true;}
bool YunZh1Jun::func253(){return true;}
bool YunZh1Jun::func254(){return true;}
bool YunZh1Jun::func255(){return true;}
bool YunZh1Jun::func256(){return true;}
bool YunZh1Jun::func257(){return true;}
bool YunZh1Jun::func258(){return true;}
bool YunZh1Jun::func259(){return true;}
bool YunZh1Jun::func260(){return true;}
bool YunZh1Jun::func261(){return true;}
bool YunZh1Jun::func262(){return true;}
bool YunZh1Jun::func263(){return true;}
bool YunZh1Jun::func264(){return true;}
bool YunZh1Jun::func265(){return true;}
bool YunZh1Jun::func266(){return true;}
bool YunZh1Jun::func267(){return true;}
bool YunZh1Jun::func268(){return true;}
bool YunZh1Jun::func269(){return true;}
bool YunZh1Jun::func270(){return true;}
bool YunZh1Jun::func271(){return true;}
bool YunZh1Jun::func272(){return true;}
bool YunZh1Jun::func273(){return true;}
bool YunZh1Jun::func274(){return true;}
bool YunZh1Jun::func275(){return true;}
bool YunZh1Jun::func276(){return true;}
bool YunZh1Jun::func277(){return true;}
bool YunZh1Jun::func278(){return true;}
bool YunZh1Jun::func279(){return true;}
bool YunZh1Jun::func280(){return true;}
bool YunZh1Jun::func281(){return true;}
bool YunZh1Jun::func282(){return true;}
bool YunZh1Jun::func283(){return true;}
bool YunZh1Jun::func284(){return true;}
bool YunZh1Jun::func285(){return true;}
bool YunZh1Jun::func286(){return true;}
bool YunZh1Jun::func287(){return true;}
bool YunZh1Jun::func288(){return true;}
bool YunZh1Jun::func289(){return true;}
bool YunZh1Jun::func290(){return true;}
bool YunZh1Jun::func291(){return true;}
bool YunZh1Jun::func292(){return true;}
bool YunZh1Jun::func293(){return true;}
bool YunZh1Jun::func294(){return true;}
bool YunZh1Jun::func295(){return true;}
bool YunZh1Jun::func296(){return true;}
bool YunZh1Jun::func297(){return true;}
bool YunZh1Jun::func298(){return true;}
bool YunZh1Jun::func299(){return true;}
bool YunZh1Jun::func300(){return true;}
bool YunZh1Jun::func301(){return true;}
bool YunZh1Jun::func302(){return true;}
bool YunZh1Jun::func303(){return true;}
bool YunZh1Jun::func304(){return true;}
bool YunZh1Jun::func305(){return true;}
bool YunZh1Jun::func306(){return true;}
bool YunZh1Jun::func307(){return true;}
bool YunZh1Jun::func308(){return true;}
bool YunZh1Jun::func309(){return true;}
bool YunZh1Jun::func310(){return true;}
bool YunZh1Jun::func311(){return true;}
bool YunZh1Jun::func312(){return true;}
bool YunZh1Jun::func313(){return true;}
bool YunZh1Jun::func314(){return true;}
bool YunZh1Jun::func315(){return true;}
bool YunZh1Jun::func316(){return true;}
bool YunZh1Jun::func317(){return true;}
bool YunZh1Jun::func318(){return true;}

void YunZh1Jun::func319()
{
    unsigned char *p_unk_51B0 = unk_51B0;
    unsigned char PC, a, b, f, r;
    unk_51B0[0] = 1;
    for(int i = 0; i < 5; i++) {
        PC = *p_unk_51B0;
        a = p_unk_51B0[PC + 1];
        b = p_unk_51B0[PC];
        r = p_unk_51B0[PC + 2];
		*p_unk_51B0 = PC + 3;
        f = ~(p_unk_51B0[b] & p_unk_51B0[a]);
        p_unk_51B0[r] = f;
    }
}

bool YunZh1Jun::func320(){return true;}
bool YunZh1Jun::func321(){return true;}
bool YunZh1Jun::func322(){return true;}
bool YunZh1Jun::func323(){return true;}
bool YunZh1Jun::func324(){return true;}
bool YunZh1Jun::func325(){return true;}
bool YunZh1Jun::func326(){return true;}
bool YunZh1Jun::func327(){return true;}
bool YunZh1Jun::func328(){return true;}
bool YunZh1Jun::func329(){return true;}
bool YunZh1Jun::func330(){return true;}
bool YunZh1Jun::func331(){return true;}
bool YunZh1Jun::func332(){return true;}
bool YunZh1Jun::func333(){return true;}
bool YunZh1Jun::func334(){return true;}
bool YunZh1Jun::func335(){return true;}
bool YunZh1Jun::func336(){return true;}
bool YunZh1Jun::func337(){return true;}
bool YunZh1Jun::func338(){return true;}
bool YunZh1Jun::func339(){return true;}
bool YunZh1Jun::func340(){return true;}
bool YunZh1Jun::func341(){return true;}
bool YunZh1Jun::func342(){return true;}
bool YunZh1Jun::func343(){return true;}
bool YunZh1Jun::func344(){return true;}
bool YunZh1Jun::func345(){return true;}
bool YunZh1Jun::func346(){return true;}
bool YunZh1Jun::func347(){return true;}
bool YunZh1Jun::func348(){return true;}
bool YunZh1Jun::func349(){return true;}
bool YunZh1Jun::func350(){return true;}
bool YunZh1Jun::func351(){return true;}
bool YunZh1Jun::func352(){return true;}
bool YunZh1Jun::func353(){return true;}
bool YunZh1Jun::func354(){return true;}
bool YunZh1Jun::func355(){return true;}
bool YunZh1Jun::func356(){return true;}
bool YunZh1Jun::func357(){return true;}
bool YunZh1Jun::func358(){return true;}
bool YunZh1Jun::func359(){return true;}
bool YunZh1Jun::func360(){return true;}
bool YunZh1Jun::func361(){return true;}
bool YunZh1Jun::func362(){return true;}
bool YunZh1Jun::func363(){return true;}
bool YunZh1Jun::func364(){return true;}
bool YunZh1Jun::func365(){return true;}
bool YunZh1Jun::func366(){return true;}
bool YunZh1Jun::func367(){return true;}
bool YunZh1Jun::func368(){return true;}
bool YunZh1Jun::func369(){return true;}
bool YunZh1Jun::func370(){return true;}
bool YunZh1Jun::func371(){return true;}
bool YunZh1Jun::func372(){return true;}
bool YunZh1Jun::func373(){return true;}
bool YunZh1Jun::func374(){return true;}
bool YunZh1Jun::func375(){return true;}
bool YunZh1Jun::func376(){return true;}
bool YunZh1Jun::func377(){return true;}
bool YunZh1Jun::func378(){return true;}
bool YunZh1Jun::func379(){return true;}
bool YunZh1Jun::func380(){return true;}
bool YunZh1Jun::func381(){return true;}
bool YunZh1Jun::func382(){return true;}
bool YunZh1Jun::func383(){return true;}
bool YunZh1Jun::func384(){return true;}
bool YunZh1Jun::func385(){return true;}
bool YunZh1Jun::func386(){return true;}
bool YunZh1Jun::func387(){return true;}
bool YunZh1Jun::func388(){return true;}
bool YunZh1Jun::func389(){return true;}
bool YunZh1Jun::func390(){return true;}
bool YunZh1Jun::func391(){return true;}
bool YunZh1Jun::func392(){return true;}
bool YunZh1Jun::func393(){return true;}
bool YunZh1Jun::func394(){return true;}
bool YunZh1Jun::func395(){return true;}
bool YunZh1Jun::func396(){return true;}
bool YunZh1Jun::func397(){return true;}
bool YunZh1Jun::func398(){return true;}
bool YunZh1Jun::func399(){return true;}
bool YunZh1Jun::func400(){return true;}
bool YunZh1Jun::func401(){return true;}
bool YunZh1Jun::func402(){return true;}
bool YunZh1Jun::func403(){return true;}
bool YunZh1Jun::func404(){return true;}
bool YunZh1Jun::func405(){return true;}
bool YunZh1Jun::func406(){return true;}
bool YunZh1Jun::func407(){return true;}
bool YunZh1Jun::func408(){return true;}
bool YunZh1Jun::func409(){return true;}
bool YunZh1Jun::func410(){return true;}
bool YunZh1Jun::func411(){return true;}
bool YunZh1Jun::func412(){return true;}
bool YunZh1Jun::func413(){return true;}
bool YunZh1Jun::func414(){return true;}
bool YunZh1Jun::func415(){return true;}
bool YunZh1Jun::func416(){return true;}
bool YunZh1Jun::func417(){return true;}
bool YunZh1Jun::func418(){return true;}
bool YunZh1Jun::func419(){return true;}
bool YunZh1Jun::func420(){return true;}
bool YunZh1Jun::func421(){return true;}
bool YunZh1Jun::func422(){return true;}
bool YunZh1Jun::func423(){return true;}
bool YunZh1Jun::func424(){return true;}
bool YunZh1Jun::func425(){return true;}
bool YunZh1Jun::func426(){return true;}
bool YunZh1Jun::func427(){return true;}
bool YunZh1Jun::func428(){return true;}
bool YunZh1Jun::func429(){return true;}
bool YunZh1Jun::func430(){return true;}
bool YunZh1Jun::func431(){return true;}
bool YunZh1Jun::func432(){return true;}
bool YunZh1Jun::func433(){return true;}
bool YunZh1Jun::func434(){return true;}
bool YunZh1Jun::func435(){return true;}
bool YunZh1Jun::func436(){return true;}
bool YunZh1Jun::func437(){return true;}
bool YunZh1Jun::func438(){return true;}
bool YunZh1Jun::func439(){return true;}
bool YunZh1Jun::func440(){return true;}
bool YunZh1Jun::func441(){return true;}
bool YunZh1Jun::func442(){return true;}
bool YunZh1Jun::func443(){return true;}
bool YunZh1Jun::func444(){return true;}
bool YunZh1Jun::func445(){return true;}
bool YunZh1Jun::func446(){return true;}
bool YunZh1Jun::func447(){return true;}
bool YunZh1Jun::func448(){return true;}
bool YunZh1Jun::func449(){return true;}
bool YunZh1Jun::func450(){return true;}
bool YunZh1Jun::func451(){return true;}
bool YunZh1Jun::func452(){return true;}
bool YunZh1Jun::func453(){return true;}
bool YunZh1Jun::func454(){return true;}
bool YunZh1Jun::func455(){return true;}
bool YunZh1Jun::func456(){return true;}
bool YunZh1Jun::func457(){return true;}
bool YunZh1Jun::func458(){return true;}
bool YunZh1Jun::func459(){return true;}
bool YunZh1Jun::func460(){return true;}
bool YunZh1Jun::func461(){return true;}
bool YunZh1Jun::func462(){return true;}
bool YunZh1Jun::func463(){return true;}
bool YunZh1Jun::func464(){return true;}
bool YunZh1Jun::func465(){return true;}
bool YunZh1Jun::func466(){return true;}
bool YunZh1Jun::func467(){return true;}
bool YunZh1Jun::func468(){return true;}
bool YunZh1Jun::func469(){return true;}
bool YunZh1Jun::func470(){return true;}
bool YunZh1Jun::func471(){return true;}
bool YunZh1Jun::func472(){return true;}
bool YunZh1Jun::func473(){return true;}
bool YunZh1Jun::func474(){return true;}
bool YunZh1Jun::func475(){return true;}
bool YunZh1Jun::func476(){return true;}
bool YunZh1Jun::func477(){return true;}
bool YunZh1Jun::func478(){return true;}
bool YunZh1Jun::func479(){return true;}
bool YunZh1Jun::func480(){return true;}
bool YunZh1Jun::func481(){return true;}
bool YunZh1Jun::func482(){return true;}
bool YunZh1Jun::func483(){return true;}
bool YunZh1Jun::func484(){return true;}
bool YunZh1Jun::func485(){return true;}
bool YunZh1Jun::func486(){return true;}
bool YunZh1Jun::func487(){return true;}
bool YunZh1Jun::func488(){return true;}
bool YunZh1Jun::func489(){return true;}
bool YunZh1Jun::func490(){return true;}
bool YunZh1Jun::func491(){return true;}
bool YunZh1Jun::func492(){return true;}
bool YunZh1Jun::func493(){return true;}
bool YunZh1Jun::func494(){return true;}
bool YunZh1Jun::func495(){return true;}
bool YunZh1Jun::func496(){return true;}
bool YunZh1Jun::func497(){return true;}
bool YunZh1Jun::func498(){return true;}
bool YunZh1Jun::func499(){return true;}
bool YunZh1Jun::func500(){return true;}
bool YunZh1Jun::func501(){return true;}
bool YunZh1Jun::func502(){return true;}
bool YunZh1Jun::func503(){return true;}
bool YunZh1Jun::func504(){return true;}
bool YunZh1Jun::func505(){return true;}
bool YunZh1Jun::func506(){return true;}
bool YunZh1Jun::func507(){return true;}
bool YunZh1Jun::func508(){return true;}
bool YunZh1Jun::func509(){return true;}
bool YunZh1Jun::func510(){return true;}
bool YunZh1Jun::func511(){return true;}
bool YunZh1Jun::func512(){return true;}
bool YunZh1Jun::func513(){return true;}
bool YunZh1Jun::func514(){return true;}
bool YunZh1Jun::func515(){return true;}
bool YunZh1Jun::func516(){return true;}
bool YunZh1Jun::func517(){return true;}
bool YunZh1Jun::func518(){return true;}
bool YunZh1Jun::func519(){return true;}
bool YunZh1Jun::func520(){return true;}
bool YunZh1Jun::func521(){return true;}
bool YunZh1Jun::func522(){return true;}
bool YunZh1Jun::func523(){return true;}
bool YunZh1Jun::func524(){return true;}
bool YunZh1Jun::func525(){return true;}
bool YunZh1Jun::func526(){return true;}
bool YunZh1Jun::func527(){return true;}
bool YunZh1Jun::func528(){return true;}
bool YunZh1Jun::func529(){return true;}
bool YunZh1Jun::func530(){return true;}
bool YunZh1Jun::func531(){return true;}
bool YunZh1Jun::func532(){return true;}
bool YunZh1Jun::func533(){return true;}
bool YunZh1Jun::func534(){return true;}
bool YunZh1Jun::func535(){return true;}
bool YunZh1Jun::func536(){return true;}
bool YunZh1Jun::func537(){return true;}
bool YunZh1Jun::func538(){return true;}
bool YunZh1Jun::func539(){return true;}
bool YunZh1Jun::func540(){return true;}
bool YunZh1Jun::func541(){return true;}
bool YunZh1Jun::func542(){return true;}
bool YunZh1Jun::func543(){return true;}
bool YunZh1Jun::func544(){return true;}
bool YunZh1Jun::func545(){return true;}
bool YunZh1Jun::func546(){return true;}
bool YunZh1Jun::func547(){return true;}
bool YunZh1Jun::func548(){return true;}
bool YunZh1Jun::func549(){return true;}
bool YunZh1Jun::func550(){return true;}
bool YunZh1Jun::func551(){return true;}
bool YunZh1Jun::func552(){return true;}
bool YunZh1Jun::func553(){return true;}
bool YunZh1Jun::func554(){return true;}
bool YunZh1Jun::func555(){return true;}
bool YunZh1Jun::func556(){return true;}
bool YunZh1Jun::func557(){return true;}
bool YunZh1Jun::func558(){return true;}
bool YunZh1Jun::func559(){return true;}
bool YunZh1Jun::func560(){return true;}
bool YunZh1Jun::func561(){return true;}
bool YunZh1Jun::func562(){return true;}
bool YunZh1Jun::func563(){return true;}
bool YunZh1Jun::func564(){return true;}
bool YunZh1Jun::func565(){return true;}
bool YunZh1Jun::func566(){return true;}
bool YunZh1Jun::func567(){return true;}
bool YunZh1Jun::func568(){return true;}
bool YunZh1Jun::func569(){return true;}
bool YunZh1Jun::func570(){return true;}
bool YunZh1Jun::func571(){return true;}
bool YunZh1Jun::func572(){return true;}
bool YunZh1Jun::func573(){return true;}
bool YunZh1Jun::func574(){return true;}
bool YunZh1Jun::func575(){return true;}
bool YunZh1Jun::func576(){return true;}
bool YunZh1Jun::func577(){return true;}
bool YunZh1Jun::func578(){return true;}
bool YunZh1Jun::func579(){return true;}
bool YunZh1Jun::func580(){return true;}
bool YunZh1Jun::func581(){return true;}
bool YunZh1Jun::func582(){return true;}
bool YunZh1Jun::func583(){return true;}
bool YunZh1Jun::func584(){return true;}
bool YunZh1Jun::func585(){return true;}
bool YunZh1Jun::func586(){return true;}
bool YunZh1Jun::func587(){return true;}
bool YunZh1Jun::func588(){return true;}
bool YunZh1Jun::func589(){return true;}
bool YunZh1Jun::func590(){return true;}
bool YunZh1Jun::func591(){return true;}
bool YunZh1Jun::func592(){return true;}
bool YunZh1Jun::func593(){return true;}
bool YunZh1Jun::func594(){return true;}
bool YunZh1Jun::func595(){return true;}
bool YunZh1Jun::func596(){return true;}
bool YunZh1Jun::func597(){return true;}
bool YunZh1Jun::func598(){return true;}
bool YunZh1Jun::func599(){return true;}
bool YunZh1Jun::func600(){return true;}
bool YunZh1Jun::func601(){return true;}
bool YunZh1Jun::func602(){return true;}
bool YunZh1Jun::func603(){return true;}
bool YunZh1Jun::func604(){return true;}
bool YunZh1Jun::func605(){return true;}
bool YunZh1Jun::func606(){return true;}
bool YunZh1Jun::func607(){return true;}
bool YunZh1Jun::func608(){return true;}
bool YunZh1Jun::func609(){return true;}
bool YunZh1Jun::func610(){return true;}
bool YunZh1Jun::func611(){return true;}
bool YunZh1Jun::func612(){return true;}
bool YunZh1Jun::func613(){return true;}
bool YunZh1Jun::func614(){return true;}
bool YunZh1Jun::func615(){return true;}
bool YunZh1Jun::func616(){return true;}
bool YunZh1Jun::func617(){return true;}
bool YunZh1Jun::func618(){return true;}
bool YunZh1Jun::func619(){return true;}
bool YunZh1Jun::func620(){return true;}
bool YunZh1Jun::func621(){return true;}
bool YunZh1Jun::func622(){return true;}
bool YunZh1Jun::func623(){return true;}
bool YunZh1Jun::func624(){return true;}
bool YunZh1Jun::func625(){return true;}
bool YunZh1Jun::func626(){return true;}
bool YunZh1Jun::func627(){return true;}
bool YunZh1Jun::func628(){return true;}

bool YunZh1Jun::func629()
{
    unsigned char check_num[30] = {100, 4, 101, 15, 44, 93, 57, 35, 35, 0, 22, 5, 29, 143, 147, 154, 160, 179, 147, 169, 146, 160, 175, 203, 140, 202};

    for(int j = 0; j<26;j++)
    if(unk_51B0[97+j]!=check_num[j])
    {    
        //outs() << j;
        return false;
    }
    return true;
}

bool YunZh1Jun::func630(){return true;}
bool YunZh1Jun::func631(){return true;}
bool YunZh1Jun::func632(){return true;}
bool YunZh1Jun::func633(){return true;}
bool YunZh1Jun::func634(){return true;}
bool YunZh1Jun::func635(){return true;}
bool YunZh1Jun::func636(){return true;}
bool YunZh1Jun::func637(){return true;}
bool YunZh1Jun::func638(){return true;}
bool YunZh1Jun::func639(){return true;}
bool YunZh1Jun::func640(){return true;}
bool YunZh1Jun::func641(){return true;}
bool YunZh1Jun::func642(){return true;}
bool YunZh1Jun::func643(){return true;}
bool YunZh1Jun::func644(){return true;}
bool YunZh1Jun::func645(){return true;}
bool YunZh1Jun::func646(){return true;}
bool YunZh1Jun::func647(){return true;}
bool YunZh1Jun::func648(){return true;}
bool YunZh1Jun::func649(){return true;}
bool YunZh1Jun::func650(){return true;}
bool YunZh1Jun::func651(){return true;}
bool YunZh1Jun::func652(){return true;}
bool YunZh1Jun::func653(){return true;}
bool YunZh1Jun::func654(){return true;}
bool YunZh1Jun::func655(){return true;}
bool YunZh1Jun::func656(){return true;}
bool YunZh1Jun::func657(){return true;}
bool YunZh1Jun::func658(){return true;}
bool YunZh1Jun::func659(){return true;}
bool YunZh1Jun::func660(){return true;}
bool YunZh1Jun::func661(){return true;}
bool YunZh1Jun::func662(){return true;}
bool YunZh1Jun::func663(){return true;}
bool YunZh1Jun::func664(){return true;}
bool YunZh1Jun::func665(){return true;}
bool YunZh1Jun::func666(){return true;}
bool YunZh1Jun::func667(){return true;}
bool YunZh1Jun::func668(){return true;}
bool YunZh1Jun::func669(){return true;}
bool YunZh1Jun::func670(){return true;}
bool YunZh1Jun::func671(){return true;}
bool YunZh1Jun::func672(){return true;}
bool YunZh1Jun::func673(){return true;}
bool YunZh1Jun::func674(){return true;}
bool YunZh1Jun::func675(){return true;}
bool YunZh1Jun::func676(){return true;}
bool YunZh1Jun::func677(){return true;}
bool YunZh1Jun::func678(){return true;}
bool YunZh1Jun::func679(){return true;}
bool YunZh1Jun::func680(){return true;}
bool YunZh1Jun::func681(){return true;}
bool YunZh1Jun::func682(){return true;}
bool YunZh1Jun::func683(){return true;}
bool YunZh1Jun::func684(){return true;}
bool YunZh1Jun::func685(){return true;}
bool YunZh1Jun::func686(){return true;}
bool YunZh1Jun::func687(){return true;}
bool YunZh1Jun::func688(){return true;}
bool YunZh1Jun::func689(){return true;}
bool YunZh1Jun::func690(){return true;}
bool YunZh1Jun::func691(){return true;}
bool YunZh1Jun::func692(){return true;}
bool YunZh1Jun::func693(){return true;}
bool YunZh1Jun::func694(){return true;}
bool YunZh1Jun::func695(){return true;}
bool YunZh1Jun::func696(){return true;}
bool YunZh1Jun::func697(){return true;}
bool YunZh1Jun::func698(){return true;}
bool YunZh1Jun::func699(){return true;}
bool YunZh1Jun::func700(){return true;}
bool YunZh1Jun::func701(){return true;}
bool YunZh1Jun::func702(){return true;}
bool YunZh1Jun::func703(){return true;}
bool YunZh1Jun::func704(){return true;}
bool YunZh1Jun::func705(){return true;}
bool YunZh1Jun::func706(){return true;}
bool YunZh1Jun::func707(){return true;}
bool YunZh1Jun::func708(){return true;}
bool YunZh1Jun::func709(){return true;}
bool YunZh1Jun::func710(){return true;}
bool YunZh1Jun::func711(){return true;}
bool YunZh1Jun::func712(){return true;}
bool YunZh1Jun::func713(){return true;}
bool YunZh1Jun::func714(){return true;}
bool YunZh1Jun::func715(){return true;}
bool YunZh1Jun::func716(){return true;}
bool YunZh1Jun::func717(){return true;}
bool YunZh1Jun::func718(){return true;}
bool YunZh1Jun::func719(){return true;}
bool YunZh1Jun::func720(){return true;}
bool YunZh1Jun::func721(){return true;}
bool YunZh1Jun::func722(){return true;}
bool YunZh1Jun::func723(){return true;}
bool YunZh1Jun::func724(){return true;}
bool YunZh1Jun::func725(){return true;}
bool YunZh1Jun::func726(){return true;}
bool YunZh1Jun::func727(){return true;}
bool YunZh1Jun::func728(){return true;}
bool YunZh1Jun::func729(){return true;}
bool YunZh1Jun::func730(){return true;}
bool YunZh1Jun::func731(){return true;}
bool YunZh1Jun::func732(){return true;}
bool YunZh1Jun::func733(){return true;}
bool YunZh1Jun::func734(){return true;}
bool YunZh1Jun::func735(){return true;}
bool YunZh1Jun::func736(){return true;}
bool YunZh1Jun::func737(){return true;}
bool YunZh1Jun::func738(){return true;}
bool YunZh1Jun::func739(){return true;}
bool YunZh1Jun::func740(){return true;}
bool YunZh1Jun::func741(){return true;}
bool YunZh1Jun::func742(){return true;}
bool YunZh1Jun::func743(){return true;}
bool YunZh1Jun::func744(){return true;}
bool YunZh1Jun::func745(){return true;}
bool YunZh1Jun::func746(){return true;}
bool YunZh1Jun::func747(){return true;}
bool YunZh1Jun::func748(){return true;}
bool YunZh1Jun::func749(){return true;}
bool YunZh1Jun::func750(){return true;}
bool YunZh1Jun::func751(){return true;}
bool YunZh1Jun::func752(){return true;}
bool YunZh1Jun::func753(){return true;}
bool YunZh1Jun::func754(){return true;}
bool YunZh1Jun::func755(){return true;}
bool YunZh1Jun::func756(){return true;}
bool YunZh1Jun::func757(){return true;}
bool YunZh1Jun::func758(){return true;}
bool YunZh1Jun::func759(){return true;}
bool YunZh1Jun::func760(){return true;}
bool YunZh1Jun::func761(){return true;}
bool YunZh1Jun::func762(){return true;}
bool YunZh1Jun::func763(){return true;}
bool YunZh1Jun::func764(){return true;}
bool YunZh1Jun::func765(){return true;}
bool YunZh1Jun::func766(){return true;}
bool YunZh1Jun::func767(){return true;}
bool YunZh1Jun::func768(){return true;}
bool YunZh1Jun::func769(){return true;}
bool YunZh1Jun::func770(){return true;}
bool YunZh1Jun::func771(){return true;}
bool YunZh1Jun::func772(){return true;}
bool YunZh1Jun::func773(){return true;}
bool YunZh1Jun::func774(){return true;}
bool YunZh1Jun::func775(){return true;}
bool YunZh1Jun::func776(){return true;}
bool YunZh1Jun::func777(){return true;}
bool YunZh1Jun::func778(){return true;}
bool YunZh1Jun::func779(){return true;}
bool YunZh1Jun::func780(){return true;}
bool YunZh1Jun::func781(){return true;}
bool YunZh1Jun::func782(){return true;}
bool YunZh1Jun::func783(){return true;}
bool YunZh1Jun::func784(){return true;}
bool YunZh1Jun::func785(){return true;}
bool YunZh1Jun::func786(){return true;}
bool YunZh1Jun::func787(){return true;}
bool YunZh1Jun::func788(){return true;}
bool YunZh1Jun::func789(){return true;}
bool YunZh1Jun::func790(){return true;}
bool YunZh1Jun::func791(){return true;}
bool YunZh1Jun::func792(){return true;}
bool YunZh1Jun::func793(){return true;}
bool YunZh1Jun::func794(){return true;}
bool YunZh1Jun::func795(){return true;}
bool YunZh1Jun::func796(){return true;}
bool YunZh1Jun::func797(){return true;}
bool YunZh1Jun::func798(){return true;}
bool YunZh1Jun::func799(){return true;}
bool YunZh1Jun::func800(){return true;}
bool YunZh1Jun::func801(){return true;}
bool YunZh1Jun::func802(){return true;}
bool YunZh1Jun::func803(){return true;}
bool YunZh1Jun::func804(){return true;}
bool YunZh1Jun::func805(){return true;}
bool YunZh1Jun::func806(){return true;}
bool YunZh1Jun::func807(){return true;}
bool YunZh1Jun::func808(){return true;}
bool YunZh1Jun::func809(){return true;}
bool YunZh1Jun::func810(){return true;}
bool YunZh1Jun::func811(){return true;}
bool YunZh1Jun::func812(){return true;}
bool YunZh1Jun::func813(){return true;}
bool YunZh1Jun::func814(){return true;}
bool YunZh1Jun::func815(){return true;}
bool YunZh1Jun::func816(){return true;}
bool YunZh1Jun::func817(){return true;}
bool YunZh1Jun::func818(){return true;}
bool YunZh1Jun::func819(){return true;}
bool YunZh1Jun::func820(){return true;}
bool YunZh1Jun::func821(){return true;}
bool YunZh1Jun::func822(){return true;}
bool YunZh1Jun::func823(){return true;}
bool YunZh1Jun::func824(){return true;}
bool YunZh1Jun::func825(){return true;}
bool YunZh1Jun::func826(){return true;}
bool YunZh1Jun::func827(){return true;}
bool YunZh1Jun::func828(){return true;}
bool YunZh1Jun::func829(){return true;}
bool YunZh1Jun::func830(){return true;}
bool YunZh1Jun::func831(){return true;}
bool YunZh1Jun::func832(){return true;}
bool YunZh1Jun::func833(){return true;}
bool YunZh1Jun::func834(){return true;}
bool YunZh1Jun::func835(){return true;}
bool YunZh1Jun::func836(){return true;}
bool YunZh1Jun::func837(){return true;}
bool YunZh1Jun::func838(){return true;}
bool YunZh1Jun::func839(){return true;}
bool YunZh1Jun::func840(){return true;}
bool YunZh1Jun::func841(){return true;}
bool YunZh1Jun::func842(){return true;}
bool YunZh1Jun::func843(){return true;}
bool YunZh1Jun::func844(){return true;}
bool YunZh1Jun::func845(){return true;}
bool YunZh1Jun::func846(){return true;}
bool YunZh1Jun::func847(){return true;}
bool YunZh1Jun::func848(){return true;}
bool YunZh1Jun::func849(){return true;}
bool YunZh1Jun::func850(){return true;}
bool YunZh1Jun::func851(){return true;}
bool YunZh1Jun::func852(){return true;}
bool YunZh1Jun::func853(){return true;}
bool YunZh1Jun::func854(){return true;}
bool YunZh1Jun::func855(){return true;}
bool YunZh1Jun::func856(){return true;}
bool YunZh1Jun::func857(){return true;}
bool YunZh1Jun::func858(){return true;}
bool YunZh1Jun::func859(){return true;}
bool YunZh1Jun::func860(){return true;}
bool YunZh1Jun::func861(){return true;}
bool YunZh1Jun::func862(){return true;}
bool YunZh1Jun::func863(){return true;}
bool YunZh1Jun::func864(){return true;}
bool YunZh1Jun::func865(){return true;}
bool YunZh1Jun::func866(){return true;}
bool YunZh1Jun::func867(){return true;}
bool YunZh1Jun::func868(){return true;}
bool YunZh1Jun::func869(){return true;}
bool YunZh1Jun::func870(){return true;}
bool YunZh1Jun::func871(){return true;}
bool YunZh1Jun::func872(){return true;}
bool YunZh1Jun::func873(){return true;}
bool YunZh1Jun::func874(){return true;}
bool YunZh1Jun::func875(){return true;}
bool YunZh1Jun::func876(){return true;}
bool YunZh1Jun::func877(){return true;}
bool YunZh1Jun::func878(){return true;}
bool YunZh1Jun::func879(){return true;}
bool YunZh1Jun::func880(){return true;}
bool YunZh1Jun::func881(){return true;}
bool YunZh1Jun::func882(){return true;}
bool YunZh1Jun::func883(){return true;}
bool YunZh1Jun::func884(){return true;}
bool YunZh1Jun::func885(){return true;}
bool YunZh1Jun::func886(){return true;}
bool YunZh1Jun::func887(){return true;}
bool YunZh1Jun::func888(){return true;}
bool YunZh1Jun::func889(){return true;}
bool YunZh1Jun::func890(){return true;}
bool YunZh1Jun::func891(){return true;}
bool YunZh1Jun::func892(){return true;}
bool YunZh1Jun::func893(){return true;}
bool YunZh1Jun::func894(){return true;}
bool YunZh1Jun::func895(){return true;}
bool YunZh1Jun::func896(){return true;}
bool YunZh1Jun::func897(){return true;}
bool YunZh1Jun::func898(){return true;}
bool YunZh1Jun::func899(){return true;}
bool YunZh1Jun::func900(){return true;}
bool YunZh1Jun::func901(){return true;}
bool YunZh1Jun::func902(){return true;}
bool YunZh1Jun::func903(){return true;}
bool YunZh1Jun::func904(){return true;}
bool YunZh1Jun::func905(){return true;}
bool YunZh1Jun::func906(){return true;}
bool YunZh1Jun::func907(){return true;}
bool YunZh1Jun::func908(){return true;}
bool YunZh1Jun::func909(){return true;}
bool YunZh1Jun::func910(){return true;}
bool YunZh1Jun::func911(){return true;}
bool YunZh1Jun::func912(){return true;}
bool YunZh1Jun::func913(){return true;}
bool YunZh1Jun::func914(){return true;}
bool YunZh1Jun::func915(){return true;}
bool YunZh1Jun::func916(){return true;}
bool YunZh1Jun::func917(){return true;}
bool YunZh1Jun::func918(){return true;}
bool YunZh1Jun::func919(){return true;}
bool YunZh1Jun::func920(){return true;}
bool YunZh1Jun::func921(){return true;}
bool YunZh1Jun::func922(){return true;}
bool YunZh1Jun::func923(){return true;}
bool YunZh1Jun::func924(){return true;}
bool YunZh1Jun::func925(){return true;}
bool YunZh1Jun::func926(){return true;}
bool YunZh1Jun::func927(){return true;}
bool YunZh1Jun::func928(){return true;}
bool YunZh1Jun::func929(){return true;}
bool YunZh1Jun::func930(){return true;}
bool YunZh1Jun::func931(){return true;}
bool YunZh1Jun::func932(){return true;}
bool YunZh1Jun::func933(){return true;}
bool YunZh1Jun::func934(){return true;}
bool YunZh1Jun::func935(){return true;}
bool YunZh1Jun::func936(){return true;}
bool YunZh1Jun::func937(){return true;}
bool YunZh1Jun::func938(){return true;}
bool YunZh1Jun::func939(){return true;}
bool YunZh1Jun::func940(){return true;}
bool YunZh1Jun::func941(){return true;}
bool YunZh1Jun::func942(){return true;}
bool YunZh1Jun::func943(){return true;}
bool YunZh1Jun::func944(){return true;}
bool YunZh1Jun::func945(){return true;}
bool YunZh1Jun::func946(){return true;}
bool YunZh1Jun::func947(){return true;}
bool YunZh1Jun::func948(){return true;}
bool YunZh1Jun::func949(){return true;}
bool YunZh1Jun::func950(){return true;}
bool YunZh1Jun::func951(){return true;}
bool YunZh1Jun::func952(){return true;}
bool YunZh1Jun::func953(){return true;}
bool YunZh1Jun::func954(){return true;}
bool YunZh1Jun::func955(){return true;}
bool YunZh1Jun::func956(){return true;}
bool YunZh1Jun::func957(){return true;}
bool YunZh1Jun::func958(){return true;}
bool YunZh1Jun::func959(){return true;}
bool YunZh1Jun::func960(){return true;}
bool YunZh1Jun::func961(){return true;}
bool YunZh1Jun::func962(){return true;}
bool YunZh1Jun::func963(){return true;}
bool YunZh1Jun::func964(){return true;}
bool YunZh1Jun::func965(){return true;}
bool YunZh1Jun::func966(){return true;}
bool YunZh1Jun::func967(){return true;}
bool YunZh1Jun::func968(){return true;}
bool YunZh1Jun::func969(){return true;}
bool YunZh1Jun::func970(){return true;}
bool YunZh1Jun::func971(){return true;}
bool YunZh1Jun::func972(){return true;}
bool YunZh1Jun::func973(){return true;}
bool YunZh1Jun::func974(){return true;}
bool YunZh1Jun::func975(){return true;}
bool YunZh1Jun::func976(){return true;}
bool YunZh1Jun::func977(){return true;}
bool YunZh1Jun::func978(){return true;}
bool YunZh1Jun::func979(){return true;}
bool YunZh1Jun::func980(){return true;}
bool YunZh1Jun::func981(){return true;}
bool YunZh1Jun::func982(){return true;}
bool YunZh1Jun::func983(){return true;}
bool YunZh1Jun::func984(){return true;}
bool YunZh1Jun::func985(){return true;}
bool YunZh1Jun::func986(){return true;}
bool YunZh1Jun::func987(){return true;}
bool YunZh1Jun::func988(){return true;}
bool YunZh1Jun::func989(){return true;}
bool YunZh1Jun::func990(){return true;}
bool YunZh1Jun::func991(){return true;}
bool YunZh1Jun::func992(){return true;}
bool YunZh1Jun::func993(){return true;}
bool YunZh1Jun::func994(){return true;}
bool YunZh1Jun::func995(){return true;}
bool YunZh1Jun::func996(){return true;}
bool YunZh1Jun::func997(){return true;}
bool YunZh1Jun::func998(){return true;}
bool YunZh1Jun::func999(){return true;}

char YunZh1Jun::ID = 0;
static RegisterPass<YunZh1Jun> X("YunZh1Jun", "YunZh1Jun's simple pass");