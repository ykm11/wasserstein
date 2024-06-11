#include <algorithm>
#include <numeric> // std::iota
#include <vector>

namespace std {
template <typename T> void argsort(const vector<T> &v, vector<size_t> &idx) {
  iota(idx.begin(), idx.end(), 0);
  // sort indexes based on values in v
  sort(idx.begin(), idx.end(),
       [&v](size_t i1, size_t i2) { return v[i1] < v[i2]; });
}

template <typename T> void diff(const vector<T> &v, vector<T> &delta) {

  delta.resize(v.size()-1);
  for (size_t i = 0; i < v.size() - 1; ++i) {
    delta[i] = v[i + 1] - v[i];
  }
}

template <typename T>
void concatenateSort(const vector<T> &A, const vector<T> &B, vector<T> &C) {
  C.reserve(A.size() + B.size());
  C.insert(C.end(), A.begin(), A.end());
  C.insert(C.end(), B.begin(), B.end());
  sort(C.begin(), C.end());
}

template <typename T>
void searchSorted(const vector<T> &v, const vector<size_t> &idx,
                  const vector<T> &allValues, vector<size_t> &cdfIdx) {
  // for every element in all indices, find id in A
  cdfIdx.resize(allValues.size());
  size_t j = 0;
  for (auto elem = allValues.begin(); elem != allValues.end() - 1; ++elem) {
    auto up = upper_bound(idx.begin(), idx.end(), *elem,
                          [&v](T elem, size_t i) { return elem < v[i]; });
    if (up == idx.end()) {
      cdfIdx[j] = idx.size();
    } else {
      cdfIdx[j] = up - idx.begin();
    }
    j++;
  }
}

template <typename T>
void computeCDF(vector<size_t> &idx,
                const vector<size_t> &cdfIdx, vector<T> &cdf) {

#if 1
  size_t j = 0;
  size_t ordW_size = idx.size();
  vector<T> ordW(ordW_size, 1); // assign the same weight
  //T accum = ordW_size;

  cdf.resize(cdfIdx.size());
  vector<T> sortedAccW = {0};
  partial_sum(ordW.begin(), ordW.end(), ordW.begin());
  sortedAccW.insert(sortedAccW.end(), ordW.begin(), ordW.end());
  for (auto cIdx : cdfIdx) {
    cdf[j] = sortedAccW[cIdx] / ordW_size;
    j++;
  }
#else
  // assign the same weight
  size_t j;
  size_t ordW_size = idx.size();
  //vector<T> ordW(ordW_size, 1); 
  //T accum = ordW_size;

  cdf.resize(cdfIdx.size());
  vector<T> sortedAccW(1 + ordW_size);
  for (j = 0; j < ordW_size + 1; j++) {
    sortedAccW[j] = (T)(j);
  }
  //vector<T> sortedAccW = {0};
  //partial_sum(ordW.begin(), ordW.end(), ordW.begin());
  //sortedAccW.insert(sortedAccW.end(), ordW.begin(), ordW.end());

  j = 0;
  for (auto cIdx : cdfIdx) {
    cdf[j] = sortedAccW[cIdx] / ordW_size;
    j++;
  }
#endif
}

template <typename T>
void computeCDF(const vector<T> &weights, vector<size_t> &idx,
                const vector<size_t> &cdfIdx, vector<T> &cdf) {

  vector<T> ordW(idx.size());
  size_t j = 0;
  for (auto i : idx) {
    ordW[j] = weights[i];
    j++;
  }

  j = 0;
  cdf.resize(cdfIdx.size());
  vector<T> sortedAccW = {0};
  partial_sum(ordW.begin(), ordW.end(), ordW.begin());
  sortedAccW.insert(sortedAccW.end(), ordW.begin(), ordW.end());
  T accum = sortedAccW.back();
  for (auto cIdx : cdfIdx) {
    cdf[j] = sortedAccW[cIdx] / accum;
    j++;
  }

}

template <typename T>
T computeDist(const vector<T> &cdfA, const vector<T> &cdfB,
              const vector<T> &deltas) {

  T dist = 0.0;
  for (size_t i = 0; i < deltas.size(); ++i) {
    dist += abs(cdfA[i] - cdfB[i]) * deltas[i];
  }
  return dist;
}

template <typename T>
T wasserstein(vector<T> &A, vector<T> &B) {

  vector<T> allValues;
  concatenateSort(A, B, allValues);

  vector<T> deltas;
  diff(allValues, deltas);

  vector<size_t> idxA(A.size());
  argsort(A, idxA);

  vector<size_t> idxB(B.size());
  argsort(B, idxB);

  vector<size_t> cdfIdxA;
  searchSorted(A, idxA, allValues, cdfIdxA);

  vector<size_t> cdfIdxB;
  searchSorted(B, idxB, allValues, cdfIdxB);

  vector<T> cdfA;
  computeCDF(idxA, cdfIdxA, cdfA);

  vector<T> cdfB;
  computeCDF(idxB, cdfIdxB, cdfB);

  return computeDist(cdfA, cdfB, deltas);
}

template <typename T>
T wasserstein(vector<T> &A, vector<T> AWeights, vector<T> &B,
              vector<T> BWeights) {

  vector<T> allValues;
  concatenateSort(A, B, allValues);

  vector<T> deltas;
  diff(allValues, deltas);

  vector<size_t> idxA(A.size());
  argsort(A, idxA);

  vector<size_t> idxB(B.size());
  argsort(B, idxB);

  vector<size_t> cdfIdxA;
  searchSorted(A, idxA, allValues, cdfIdxA);

  vector<size_t> cdfIdxB;
  searchSorted(B, idxB, allValues, cdfIdxB);

  vector<T> cdfA;
  computeCDF(AWeights, idxA, cdfIdxA, cdfA);

  vector<T> cdfB;
  computeCDF(BWeights, idxB, cdfIdxB, cdfB);

  return computeDist(cdfA, cdfB, deltas);
}
} // namespace std
