#ifndef MULTIMAPWDEF
#define MULTIMAPWDEF

#include <multimap>

using namespace std;

// multmap that still lets you use [] to reference FIRST
// entry of a key if multiples exist
template <class Key, class T, class Compare>
class multimapwithdefault : public multimap<Key, T, Compare> {
public:
	typedef pair<const Key, T> value_type;
	T& operator[](const Key& k) {
		if (find(k) == end()) {
			insert(value_type(k, T()));
		}
		return (*(find(k))).second;
	}
	bool has(const Key& k, const T &val) const {
		multimap<Key, T, Compare>::const_iterator start = lower_bound(k);
		multimap<Key, T, Compare>::const_iterator end = upper_bound(k);
		for (; start!=end; start++) {
			if (start->second == val)
				return true;
		}
		return false;
	}
};

#endif
