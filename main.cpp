#include <bits/stdc++.h>
using namespace std;

struct FastIO {
    static const size_t BUFSIZE = 1 << 20;
    int idx, size;
    char buf[BUFSIZE];
    FastIO(): idx(0), size(0) {}
    inline char read() {
        if (idx >= size) {
            size = (int)fread(buf, 1, BUFSIZE, stdin);
            idx = 0;
            if (size == 0) return 0;
        }
        return buf[idx++];
    }
    template<typename T>
    bool nextInt(T &out) {
        char c; T sign = 1; T val = 0;
        c = read();
        if (!c) return false;
        // skip non-digit
        while (c && (c!='-' && (c<'0'||c>'9'))) c = read();
        if (!c) return false;
        if (c == '-') { sign = -1; c = read(); }
        for (; c && (c>='0'&&c<='9'); c = read()) val = val*10 + (c - '0');
        out = val * sign;
        return true;
    }
} io;

struct SegTree {
    int n;
    vector<int> mn;
    vector<int> lazy;
    SegTree(int n=0, int init=0): n(n), mn(n*4), lazy(n*4) {
        if (n>0) build(1,1,n,init);
    }
    void build(int p, int l, int r, int init) {
        lazy[p] = 0;
        if (l==r) { mn[p] = init; return; }
        int m=(l+r)>>1;
        build(p<<1,l,m,init);
        build(p<<1|1,m+1,r,init);
        mn[p] = min(mn[p<<1], mn[p<<1|1]);
    }
    inline void apply(int p, int val) {
        mn[p] += val;
        lazy[p] += val;
    }
    inline void push(int p) {
        if (lazy[p]!=0) {
            apply(p<<1, lazy[p]);
            apply(p<<1|1, lazy[p]);
            lazy[p] = 0;
        }
    }
    void range_add(int p, int l, int r, int ql, int qr, int val) {
        if (ql<=l && r<=qr) { apply(p,val); return; }
        push(p);
        int m=(l+r)>>1;
        if (ql<=m) range_add(p<<1,l,m,ql,qr,val);
        if (qr>m) range_add(p<<1|1,m+1,r,ql,qr,val);
        mn[p] = min(mn[p<<1], mn[p<<1|1]);
    }
    int range_min(int p, int l, int r, int ql, int qr) {
        if (ql<=l && r<=qr) return mn[p];
        push(p);
        int m=(l+r)>>1;
        int res = INT_MAX;
        if (ql<=m) res = min(res, range_min(p<<1,l,m,ql,qr));
        if (qr>m) res = min(res, range_min(p<<1|1,m+1,r,ql,qr));
        return res;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int x,k,p;
    if (!io.nextInt(x)) return 0;
    io.nextInt(k);
    io.nextInt(p);

    // Number of segments between stations
    int nseg = max(0, x-1);

    string out;
    out.reserve((size_t)p*2 + 16);

    if (nseg == 0) {
        // No travel possible, all zero-length trips are trivially satisfied
        for (int i=0;i<p;i++){
            int u,v,n; io.nextInt(u); io.nextInt(v); io.nextInt(n);
            out.push_back('T'); out.push_back('\n');
        }
        fwrite(out.data(), 1, out.size(), stdout);
        return 0;
    }

    SegTree st(nseg, k);

    for (int i=0;i<p;i++){
        int u,v,need;
        io.nextInt(u); io.nextInt(v); io.nextInt(need);
        if (need<=0 || u>=v) {
            out.push_back('T'); out.push_back('\n');
            continue;
        }
        // map stations to segment indices [u, v-1] -> [u, v-1] in 1-based
        int l = max(1, u);
        int r = min(nseg, v-1);
        if (l>r) { out.push_back('T'); out.push_back('\n'); continue; }
        int mn = st.range_min(1,1,nseg,l,r);
        if (mn >= need) {
            st.range_add(1,1,nseg,l,r, -need);
            out.push_back('T'); out.push_back('\n');
        } else {
            out.push_back('N'); out.push_back('\n');
        }
    }

    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}

