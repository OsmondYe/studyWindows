#include "pch.h"
#include "helper.hpp"

/*
leet code 1207
*/
bool uniqueOccurrences(vector<int> arr) {
    map<int, int> cache;
    for (auto i : arr) {
        cache[i]++;
    }
    if (cache.empty()) {
        return true;
    }
    vector<int> counts;

    for (auto i : cache) {
        counts.push_back(i.second);
    }
    std::sort(counts.begin(), counts.end());
    return std::adjacent_find(counts.begin(), counts.end()) == counts.end();

}
TEST(AlgoHash, UNOO) {
    aux::println("Unique Number of Occurrences");
    EXPECT_TRUE(uniqueOccurrences({ 1,2,2,1,1,3 }));
    EXPECT_TRUE(!uniqueOccurrences({ 1,2 }));
    EXPECT_TRUE(uniqueOccurrences({ -3,0,1,-3,1,1,1,-3,10,0 }));
}

/*
205. Isomorphic Strings
Input: s = "egg", t = "add"
Output: true    e->a  g->d
*/
bool isIsomorphic(string s, string t) {
    // sanity check
    if (s.empty() && t.empty()) {
        return true;
    }
    if (s.empty() && !t.empty()) {
        return false;
    }
    if (!s.empty() && t.empty()) {
        return false;
    }

    unordered_map<char, char> s2t;
    unordered_map<char, char> t2s;
    s2t[s[0]] = t[0];
    t2s[t[0]] = s[0];

	for (int i = 1; i < s.size(); ++i) {
		char l = s[i], r = t[i];

        if (s2t.find(l) != s2t.end()) { // exist, so to check if match
            if (s2t[l] != r) {
                return false;
            }
        }
        else {// not exsit, 
            if (t2s.find(r) != t2s.end() /*&& t2s[r] == l*/) {
                return false;
            }
            s2t[l] = r;
            t2s[r] = l;
        }

	}
	return true;
   
}
TEST(AlgoHash, II) {
    aux::println("is isomorphic a->b b->a");
    EXPECT_TRUE(isIsomorphic("egg","add"));
    EXPECT_TRUE(!isIsomorphic("foo","bar"));
    EXPECT_TRUE(isIsomorphic("paper","title"));
    EXPECT_TRUE(!isIsomorphic("ab","aa"));
}
/*
leet code 217 , array contain duplicate value
*/
bool containsDuplicate(vector<int> nums) {

    unordered_set<int> cache(nums.size());
    for (auto i : nums) {
        if (cache.find(i) != cache.end()) {
            // exist;
            return true;
        }
        else {
            cache.insert(i);
        }
    }
    return false;

}

/* leet code 219;
   exist? n[j]==n[i],  j-i<=k
*/
bool containsNearbyDuplicate(vector<int> nums, int k) {
    //sanity check
    if (nums.empty()) {
        return false;
    }
    if (k == 0) {
        return false;
    }
    k = std::abs(k);
    
    // linear search
    // the problem is Time Limit Exceeded
  /*  for (int i = 0; i < nums.size(); ++i) {
        for (int j = 1; j <= k; ++j) {
            if (i+j<nums.size() && nums[i] == nums[i + j]) {
                return true;
            }
        }
    }*/

    // sliding window + cache
    unordered_set<int> cache;
    cache.reserve(k);

    //- fill cache first at min[n.size,k]
    int min = std::min((int)nums.size(), k);
    for (int i = 0; i < min; ++i) {
        if (cache.find(nums[i]) == cache.end()) {
            cache.insert(nums[i]);
        }else{
            return true;
        }
    }
    // - sliding cache as sliding window,
    for (int i = cache.size(); i < nums.size(); ++i) {
        if (cache.find(nums[i]) == cache.end()) {
            cache.erase(nums[i - cache.size()]);
            cache.insert(nums[i]);
        }
        else {
            return true;
        }
    }


    return false;

}


TEST(AlgoHash, CD) {
    aux::println("array contains duplicate");
    EXPECT_TRUE(containsDuplicate({ 1,2,3,1 }));
    EXPECT_TRUE(!containsDuplicate({ 1,2,3,4 }));
    EXPECT_TRUE(containsDuplicate({ 1,1,1,3,3,4,3,2,4,2 }));

    aux::println("arrary contains nearbyDucplicate");
    EXPECT_TRUE(containsNearbyDuplicate({ 1,2,3,1 },3));
    EXPECT_TRUE(containsNearbyDuplicate({ 1,0,1,1 },1));
    EXPECT_TRUE(containsNearbyDuplicate({ 99,99},2));  // good test-case
    EXPECT_TRUE(!containsNearbyDuplicate({ 1,2,1},0));  // good test-case
    EXPECT_TRUE(!containsNearbyDuplicate({ 1,2,3,1,2,3 },2));
}


/*
leetcode 242,  valid anagram
*/

bool isAnagram(string s, string t) {
    if (s.size() != t.size()) {
        return false;
    }
    if (s.empty() && t.empty()) {
        return true;
    }
    unordered_map<char, int>  cache;
    for (auto c : s) {
        cache[c]++;
    }
    for (auto c : t) {
        if (cache.find(c) == cache.end()) {
            return false;
        }
        else {
            if (!--cache[c]) {
                cache.erase(c);
            }
            
        }
    }
    return cache.empty();
}

TEST(AlgoHash, AG) {
    EXPECT_TRUE(isAnagram("anagram", "nagaram"));
    EXPECT_TRUE(!isAnagram("ret", "car"));
}

/*
290. Word Pattern
scan and match using cache
*/

bool wordPattern(string pattern, string str) {
    if (pattern.empty() && str.empty()) {
        return true;
    }

    unordered_map<char, string> cache;// allowed char->string
    size_t pos = 0;
    string m;
    //notice extra check, anti "ab" "dog dog"
    //notice, add extra ' ' to str.end as the sentry
    str.push_back(' ');
    for (auto p : pattern) {
        if (cache.find(p) != cache.end()) {
            m = cache[p];
            if (0 == str.compare(pos, m.size(), m)) {
                pos += m.size()+1;
            }
            else {
                return false;
            }
        }
        else {
            //insert p and m(from str) into cache
            size_t new_pos= str.find_first_of(" ", pos);
            if (new_pos == string::npos && pos==str.size()-1 ) {
                return false;
            }
            m = str.substr(pos, new_pos - pos);
            //extra check, m must not be exsit in cache.value
            if (std::any_of(cache.begin(), cache.end(), [&m](decltype(cache)::const_reference c) {return !c.second.compare(m); })) {
                return false;
            }
            cache[p] = m;
            pos = new_pos + 1;
        }
    }
    return pos==str.size();
}
// using istringstream
bool wordPattern2(string pattern, string str) {
    // splite str with each word into buf vector
    istringstream iss(str);
    vector<string> buf;
    string w;
    while (iss>>w){
        buf.push_back(w);
    }
    // sanity check
    if (pattern.empty() && buf.empty()) {
        return true;
    }
    if (pattern.size() != buf.size()) {
        return false;
    }

    // how to speed up the match,  quick scan and match
    unordered_map<char, string> c2s;
    unordered_map<string, char> s2c;
    // init value
    c2s[pattern[0]] = buf[0];
    s2c[buf[0]] = pattern[0];
    for (size_t i = 1; i < pattern.size(); ++i)
    {
        const char p = pattern[i];
        const string& m = buf[i];
        if (c2s.find(p) != c2s.end()) { // p exsit
            if (0 != m.compare(c2s[p])) {
                return false;
            }
        }
        else { // not exist 
            if (s2c.find(m) != s2c.end()) {
                return false; // anti "ab" -> "aa aa""
            }
            c2s[p] = m;
            s2c[m] = p;
        }
    }
    return true;
}

TEST(AlgoHash, WP) {
    aux::println("word pattern detect");
    EXPECT_TRUE(wordPattern("abba", "dog cat cat dog"));
    EXPECT_TRUE(!wordPattern("abba", "dog cat cat fish"));
    EXPECT_TRUE(!wordPattern("aaaa", "dog cat cat dog"));
    EXPECT_TRUE(!wordPattern("abba", "dog dog dog dog"));
    EXPECT_TRUE(wordPattern("abc", "c b a"));
    EXPECT_TRUE(!wordPattern("aaa", "aa aa aa aa"));


    EXPECT_TRUE(wordPattern2("abba", "dog cat cat dog"));
    EXPECT_TRUE(!wordPattern2("abba", "dog cat cat fish"));
    EXPECT_TRUE(!wordPattern2("aaaa", "dog cat cat dog"));
    EXPECT_TRUE(!wordPattern2("abba", "dog dog dog dog"));
    EXPECT_TRUE(wordPattern2("abc", "c b a"));
    EXPECT_TRUE(!wordPattern2("aaa", "aa aa aa aa"));

}


/*
leet-code 349, Intersection of Two Arrays
*/
vector<int> intersection(vector<int> nums1, vector<int> nums2) {
    // sanity check
    if (nums1.empty() || nums2.empty()) {
        return vector<int>();
    }
    // each int -> counts
    unordered_map<int, int> cache1;
    unordered_map<int, int> cache2;

    for (auto i : nums1) {
        cache1[i]++;
        if (cache1[i] > 1) {
            cache1[i] = 1;
        }
    }
    for (auto i : nums2) {
        cache2[i]++;
        if (cache2[i] > 1) {
            cache2[i] = 1;
        }
    }
    // move cache2 to cache1
    for (auto i : cache2) {
        cache1[i.first]++;
    }
    vector<int> rt;
    for (auto i : cache1) {
        if (i.second > 1) {
            rt.push_back(i.first);
        }
    }
    return rt;
}

vector<int> intersection2(vector<int> nums1, vector<int> nums2) {
    // sanity check
    if (nums1.empty() || nums2.empty()) {
        return vector<int>();
    }

    unordered_set<int> c1(nums1.begin(),nums1.end());
    unordered_set<int> c2(nums2.begin(), nums2.end());

    unordered_map<int, int> c3;
    for (auto i : c1) {
        c3[i]++;
    }
    for (auto i : c2) {
        c3[i]++;
    }
    vector<int> rt;
    for (auto i : c3) {
        if (i.second > 1) {
            rt.push_back(i.first);
        }
    }
    return rt;
}

TEST(AlgoHash, IOTA) {

    auto i = intersection({ 1,2,2,1 }, { 2,2 });
    cout << i[0];
}

/*387. First Unique Character in a String*/
int firstUniqChar(string s) {
    if (s.empty()) {
        return -1;
    }
    if(s.size() == 1) {
        return 0;
    }
    unordered_map<char, int> cache;
    for (auto c : s) {
        cache[c]++;
    }
    for (int i = 0; i < s.size(); ++i) {
        if (cache[s[i]] == 1) {
            return i;
        }
    }

    return -1;
}

TEST(AlgoHash, 387) {
    aux::println("first unique char in a string");
    EXPECT_EQ(0, firstUniqChar("leetcode"));
    EXPECT_EQ(2, firstUniqChar("loveleetcode"));
    EXPECT_EQ(1615, firstUniqChar("uindrseqbljlhqvlwvgdebeihttirikuahlikgnahvgnptmqwbovmuwesxkvcitcwrwrucsbbfqvldridfviduqvmfcmeiphoqupbitnwdbvevouoaetisdmgvvvwoglwtgjrpcbghxkrkjthetxeexbphbjiehaicuicgnirslhdstgmqcdnlulpdpadjdltfouwhfqicfcqntnpeqaohslwkhbvflxaudembsrsindluthxapnmrqinsivbxmkohubvmmmpmklbfrmeuvdrhptdmelmjjefgbsqqlbqhvsmswwxrlkutadqbbeisbgfrcivvoxmxxptrscxnjjvtajfhqiucdihihcutxhlonlomfdnbwanrcnbarojsajseqrgkuqgkcnvrghxnmbclfomktwfaakodeecsglufvobkgoqsbrhdiuhxqbcndkmxuertupngvnkgwrfwgdbiurbooxariklwarjgavsuddoveipltessrssxwxgvrrtkisnbavkbpaphicxhapjjpkccakepuafjdaswfwfmbdmuuaveukxvnpkgmhcjcpqntssthjlvrngugbhrivtwmbrvrprrlfmvwjdkonfmgnepqoxwfcvefjihisarwskjfmqrjlkbbugfawmkqgfxgpokkxxivqbqimwsccdekjegwcxhmmuhpstxfpofwmrmhrxeptxwbvxsesheijjfsshgrrwckjkmbslpbngnwhokjnujtiepfrdhiwwgbffixaidabacaibummwxgxowsewlqfxrkarjrkqxmxwobqbcoowmbggtpoqadhqdxlhvxrkfuwpxxgnchudreoeuefkqhlrmwwfvjexvbxdhtdngwvoohpjtdbdbesceiafrhenfljleegsencfmbauxlltfueudnjxsiiggsfwiuidgktsbtcvxtdllviocfdbonjciosucbjidwxnogmnveqkcrxbpamfxwxiugjgrfpstromuxxdiodqeoqdlfulttraquskhwfholbrcbchijlgqvuwxvejedikvgetlmrcmeampdgjvmbdovkcjilbralhmniwvbeandldnudkpjvhoqtfdwllridwljfvflfdrbqadvabggwsiexrthrngexpebuhtefkqgkkjoopsmunesfotsprxuaswwenhkdvsspkppulecahkkvccqngeaoijjgvsfqfpvphvhdnkkcqsebhijkvfpqjmarbkpejagtbtisnflrrvawmvfxeccxtrmorihgslxlqrqcmouojjfhcieuwlrcqhevmveookgonxqdbtgqjimsidnaaiuchwkfkpxfptuvhfqemojixqvhgokdekdwbomptqqlfiaiptxvgfmovdqxupjxjpoxuplsagxpgpmvtcpawkrrthvclhdbpqeucchxptdceswhnqocmeocpgthkgxmxggwlnantwwuoqpmnpvgateitxlocmhnihmfgjnvrggenbhnjfubtoteojojjkjpcnwqthxhlfukingjletwnxdnjwrgjopqoxtqcvwsakqxxumbtcblufdmdvxfkshitenmenkxjvblsoiyhjpakinimwxhcebabgsvqftfvwjnstltjawhwipkubadtoxqrutkwxjnmfoowtnvqplvqokcuwlkmxxhboampcdwokjfxggtnojebagxlwaeowtomubtbfsrufkttugfpnxmipkcsphqafjuxovwpcgonhiqmomsweunoeqkpkxxsdksmufowqpmkontccckcdbwrfwamikananakgjkahndrepemcgxecgpltvdbpoexemnrejdephuuxhfcubxlbdrmhvmeqmtdhnbkwnidigxdantmkckijiecavkpumegrbveffclcltmibjcstrrauphfxxssgxkkirapiihnlbrodvfostahqdkajqtrrfwdsemwxlucbbjjspnnqjnpnpmimhulgoskwpsactexmkfdhaihnlggqeunqevxfrpiwskhrhgfluelshqejavomjshfomvgpugqesbtakvxqwrtguuebcgqphocglfrircfvuikfbviomfrsnpvvlftwrkbmjpvdpgvohtmxcxwnuhwojnfvsfwvdlaxxlmafihpussffcawjpaxdwerwfbvsbipljualcnhseealvqiqfiaiskaafufaubvhjglktfhbdsaskmedroxkrxshnggumcbtdcablawkodmnafkjekuiecqlvbxocfcwipaicgndfafjhtjcelakrecmntjxeqjqgxkxapuobbcaxfrpsktjswxdfvugmmgjwiphnsclqwmranthpiueffaxvhplajqsrtoxbixbdpcfkbpkjkelemubwachcpxcniaqmkmasmvlfcubcdkxfkupgcgbrvmbmgnjgfbawmmdritdrkppswatwtdjemhifhmshunkvaivhteqnwkdcrpfxmrafupfhbgligbvrqjkvcgbiqudvtblhfgetffduvsfhsmjimgruxrvbqniluapaoniwhqhltbxvrphmlisfaomqoecmdbbrgujgsbdelkbddcgmpggogfonssxisphbwljlvhwhewmpqugxgbubqfeilobxxhtcxmxvjtbuavxlaghhjiemihvrsjxbleutpuumgndtnocwkpkdaupbmcsahcbwoelmgnwqummmekwhpahtdvehoprfcciwqphfwppscruimikiximhbkdomovbdalsihioncpaclevxawqcqthtmuogolkrvlipropmnmecttmlecpapdlrcqxopfjobgsvhcadqiudqjoscpvjguddnorldkpqtoocrtkcutkvdpbkkekenpqjmxmuxccamkwwxrdrbdmameptvcgsltqdasicoouvdtbranexokmtkioptqblrruaihvfvvebjigkuwwgearxvmimlgmxgpvbnotepprknjxcunheclsvxxmmoufvdhrwxqlrkmvcaxtbiqrjrnrsfrkvttnpxbspnetcofhpcbcphrchivnrskkalsvdllhtisnaerxbcxutcpmnquwrutmmvrtchhheplhdmbfkrrrxalosjbebtdqtsjbdcuuatgldnubigijbkehxsmhsadwtcbwinlnilthiqslvsduspkjuunpetcofujbqhtkmalnjmllhulckqodbgffxdiqjajecbmlekvepkirxnguqnpnfjdptinuphcoxkoqhqrmkkawmngqwvfpcuxgpekkprupplsbwshmsduaitlhcjargacduewtqgnxbtpwrkawpncarxlsnardkftmsdjflmhoplghandjisupacixxthwokctbxekqwbmhtoutgrogxsdnmcaqrsqrpigbigqhsmrksxgkdbdbxkwimpehltcffuhrphbaqgvabjduudbrxgwiljkwijvaugnleamskjijhnvcspagsfnhjielrvxddxqfgaipfflffksmvgaioaclfsjhovcotoaiwhhnmrudsqmsepsgdofnxjjcgdejutmdtgauuvuepajlikebtpgfimudswplfwgaodtwqjuaujlbqqartevsqesmrpknoanfprqcwochqdrcvquovtsogptduvfiaislfvpxwsqitodxwckprpfxtqhbdixklhtsajqgbinfpdqaehrwhnwggmggnbstolrcbmlhpfjurbknpwbrfhvdanrjcwnvsxudagbkhdlwujqvwpbbgmsfrkgutkqwxqfemaaokrfdocsbbktwaugoadgvqcdhmqkworbvxqwdpmbjkqghmapevrsnaaparwnbvcfnhhgfqplfwiswrxxistamulcbxcmvkbcxsgftbmbvqlxrhbmcujkktdvcgnjkfaclxndtjkdbgqvbqxicgmpowbjsfvspvxtrovjwbbkaapfgcqbifdosnmutvfbgdmhiqvflasedhafqnmrlmwbuhmptnkvvojnjngiepmdktewwsuxernpfmktsgwwnxfvmuotpfxfntcrsjpldmringfpkpievvbgbghevlmuticukrfvahrmtkvckjbxxrmkilbeojsxgbvbhsgvkhgtjddoreipbkwiqbkecobfexswiwttlohnfwokixfmfrpadtudrssiobbvhctpbeesiogwhdqojoalxpxikwuheilvsrxbfbnckjeswhkfviihvwrfxkxcomhvsjojhltospooumkstqqnvxmrowlrtufhxkipxnfecchklrqljfwnglxeharemawsbvnuentnapewjhnzbieisjtomdqtjkigisgopmgcfqxcxninrisupchhmfrosxgdtakhrrnbbnxovvbfjgwdgnlxoxswhsfjeijvivqemoekkmrttscthlpglaorarhibrucuhikfkphqmhowloobeumlikqmmlatesupiekwjhdvntnjbakohrbobimmujkbatfxpmgfmmqsgwsffcbenwgdrhlqrsmfghevnjfdlxupihmkptsefubmixhacnxngpjrckxhvhrelolqwevoqklhxbplakmsxdetvpuddmvaodpglmsknbvfftstckjurbntwqenuhcurxqvotrxfpootqkobeatoduihwhpjxxrvboggkcxagnrrfwfuaqplirlnafprumnkcjxlpvanfmqwuoaupqptqeulpwvnhbahbkihustenkbdushjakemufdhjllnowmvgpdxbrhxfonpgcjslvwgdmajicqqmdxubrofvdodfsedjghpbjncgkckeudxwascljlraoodwvgpvojcqgbalntslsfrwnfcdvidsdrvwhscskpalubxeobapkgpdsqcjpwkednraijmbcreplwijofniggavpdlwfmwnvsaridhbppeolakquhamneffnfmbruivassdaaikxiaxupgdpgkfojvkkagcsmqnweofwikvevrsottkbtcldoruakajinnlgxmpddcrmohaktsdrxjelrbmfdthspikxeocqqdordrqwjtaxihswcubtfomksaxddcvtadnrtqkmdnacgduudtdbhsgpfomrdiaotcfwqlxccqeelgtnkflgixpjcriroruhbbgpidwkggdevpfratqebewaxcudseaikqjuuhpsguvtwxniofskkslfnrphatmxngneuituefcxufisrsjitqxwufrgidbwlmkrqojpwpljasaiukwwthrudhcocoguerakhajdncxbrnuavoqeuwsamwdqgutbaoixcgeibpoajhedooqcewiqvddedmanxljjjcbojgbmwabkfbvamgnfpncdcxoaqhmgurifpwpgrpctlqpwmqraknjltneknsphtwbnbiahknxipsovljkivlpggvldeveeopvoqlvfjbratadttlcecomllpkdgiloeedquivsnmxkfcvrkwaohgbrbvjklkktgwtlfgafqgbigheajrvffvvkkmibcedfmnwasopoqgxjohjoqnijeaifuwiwmogkwqlfmibuwecvnulvhkbsukscqlqlsxjulillmlgjkrbmllcunhbqmbujftqgkpwkvemthigednfohxpsduotwfnknfjbexflcfieaosnlhndsorbkcdlahovwbccshmrlcofowrmqajluiqaarnqtxokbaddswftiexiahmstpbonwmhvqgcpmfrocvtwfhjrtsupscfmvwfvaoolanrlgdsvgoseltdnoxrdglockhwlvffaakgrxjfnfbxnbprfvpwmexnhfekjnkenbohhmwlqoteiwgtjsrnceptbgwkfcmtkliwwqskmsoihmnbjsvnmfkwbwemijdtpnajgrousbrdaagenqlgeaiixfgcbfhceaxkwxbpksfouuqcvcerqecpdtvtsubbadmaatdnmnqhladeiapejkxffoagbwqbvppssvvvhlvhntatxlvqgjxbejpvxemqbdjknuogumrwbognklmvjsldrktpeowiuaapbjgktxwfjiferxgmafbcerteqlvpqvxbgdwiufdctkwptadtujmifppudubpmdtiedmqhnihquansnjufpbuumhhmidphkwusjdsdaocavtauhsvtgcoqhufmacwcbxvjmagkounkoqpcnoanhgwsjvgtlwgvbpdbufekosgagfsmadmvbonkrtcbspoabugkcjeebqhqwfjcqlqjvabaqecofgwskqplgup"));
}