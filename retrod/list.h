#define LIST(type) struct type##List
#define DECLARE_LL(type) LIST(type) { type data; LIST(type)* next }
#define _LL_ELEMENT(type, data, next) (LIST(type)) { data, next }
#define CAR(ll) ll->data
#define CDR(ll) ll->next
#define LOOP_THROUGH(type, var, ll) for (LIST(type) _i_ = ll, type var = CAR(_i_); (_i_ != NULL) && (var = CAR(_i_)); _i_ = CDR(_i_))   // Fix maybe
#define LAST_ELEMENT(type, _ll) ({LIST(type) ll = _ll; while (CDR(ll) != NULL) ll = CDR(ll); ll;})   // gcc extension
#define PUSH(type, ll, x) do { if (CAR(ll) == NULL) CAR(ll) = _LL_ELEMENT(x, NULL); else CDR(LAST_ELEMENT(type, ll)) = _LL_ELEMENT(x, NULL); } while (0)