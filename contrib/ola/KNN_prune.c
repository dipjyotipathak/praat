/* KNN_prune.c
 *
 * Copyright (C) 2007-2008 Ola Söder
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * os 20070529 Initial release
 * os 20090123 Bugfix: Rejects classifiers containing 0 or 1 instances
 */

#include "KNN_prune.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// Prune                                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////////

long KNN_prune_prune
(
    ///////////////////////////////
    // Parameters                //
    ///////////////////////////////

    KNN me,     // the classifier to be pruned
                //
    double n,   // pruning degree: noise, 0 <= n <= 1
                //
    double r,   // pruning redundancy: noise, 0 <= n <= 1
                //
    long k      // k(!)
                //
)

{
    Categories uniqueCategories = Categories_selectUniqueItems(my output, 1);
    if(Categories_getSize(uniqueCategories) == my nInstances)
        return(0);

    long removals = 0;
    long ncandidates = 0;
    long candidates[my nInstances];
    double progress = 1 / (double) my nInstances;

    if(my nInstances <= 1)
        return(0);

    for (long y = 1; y <= my nInstances; y++)
    {
        if (!Melder_progress1(1 - (double) y * progress, L"Pruning noisy instances")) return(removals);
        if (KNN_prune_noisy(my input, my output, y, k))
        {
            if (n == 1 || NUMrandomUniform(0, 1) <= n)
            {
                KNN_removeInstance(me, y);
                ++removals;
            }
        }
    }

    Melder_progress1(1.0, NULL);

    for (long y = 1; y <= my nInstances; ++y)
    {
        if (!Melder_progress1(1 - (double) y * progress, L"Identifying superfluous and critical instances")) return(removals);
        if (KNN_prune_superfluous(my input, my output, y, k, 0) && !KNN_prune_critical(my input, my output, y, k))
            candidates[ncandidates++] = y;
    }

    Melder_progress1(1.0, NULL);

    KNN_prune_sort(my input, my output, k, candidates, ncandidates);
    progress  = 1 / ncandidates;

    for (long y = 0; y < ncandidates; ++y)
    {
        if (!Melder_progress1(1 - (double) y * progress, L"Pruning superfluous non-critical instances")) return(removals);
        if (KNN_prune_superfluous(my input, my output, candidates[y], k, 0) && !KNN_prune_critical(my input, my output, candidates[y], k))
        {
            if (r == 1 || NUMrandomUniform(0, 1) <= r)
            {
                KNN_removeInstance(me, candidates[y]);

                for(long i = y + 1; i < ncandidates; ++i)
                    if(candidates[i] > candidates[y])
                        --candidates[i];

                ++removals;
            }
        }
    }

    Melder_progress1(1.0, NULL);
    return(removals);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// sort indices according to pruning order defined by rule 2                               //
/////////////////////////////////////////////////////////////////////////////////////////////

void KNN_prune_sort
(
    ///////////////////////////////
    // Parameters                //
    ///////////////////////////////

    Pattern p,      // source
                    //
    Categories c,   // source
                    //
    long k,         // k(!)
                    //
    long * indices, // indices of instances to be sorted
                    //
    long nindices   // the number of instances to be sorted
                    //
)

{
    long n = nindices;
    long h[nindices];

    for (long cc = 0; cc < nindices; ++cc)
        h[cc] = KNN_friendsAmongkNeighbours(p, p, c, indices[cc], k);

    while (--n) // insertion-sort, is heap-sort worth the effort?
    {
        for (long m = n; m < nindices - 1; m++)
        {
            if (h[m - 1] > h[m]) 
                break;

            if (h[m - 1] < h[m])
            {
                OlaSWAP(long, indices[m - 1], indices[m]);
            }
            else
            {
                if (KNN_nearestEnemy(p, p, c, indices[m - 1]) < KNN_nearestEnemy(p, p, c, indices[m]))
                {
                    OlaSWAP(long, indices[m - 1], indices[m]);
                }
                else 
                {
                    if (NUMrandomUniform(0, 1) > 0.5) 
                        OlaSWAP(long, indices[m - 1], indices[m]);
                }
            }
        }
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////
// k-coverage                                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////

long KNN_prune_kCoverage
(
    ///////////////////////////////
    // Parameters                //
    ///////////////////////////////

    Pattern p,      // source
                    //
    Categories c,   // source
                    //
    long y,         // source instance index
                    //
    long k,         // k(!)
                    //
    long * indices  // Out: kCoverage set
                    //
)

{
    Melder_assert(y <= p->ny);
    Melder_assert(k > 0 && k <= p->ny);

    long cc = 0;
    FeatureWeights fws = FeatureWeights_create(p->nx);

    if (fws)
    {
        long tempindices[p->ny];
        for (long yy = 1; yy <= p->ny; yy++)
        {
            if (y != yy && FRIENDS(c->item[y], c->item[yy]))
            {
                // long n = KNN_kNeighboursSkip(p, p, fws, yy, k, tempindices, 0); .OS.081011
                long n = KNN_kNeighboursSkip(p, p, fws, yy, k, tempindices, y);
                while (n)
                {
                    if (tempindices[--n] == y)
                    {
                        indices[cc++] = yy;
                        break;
                    }
                }
            }
        }
        forget(fws);
    }
    return(cc);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// testing for superfluousness                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////

int KNN_prune_superfluous
(
    ///////////////////////////////
    // Parameters                //
    ///////////////////////////////

    Pattern p,      // source
                    //
    Categories c,   // source
                    //
    long y,         // source instance index
                    //
    long k,         // k(!)
                    //
    long skipper    // Skipping instance skipper
                    //
)

{
    if (y > p->ny) y = p->ny;   // safety belt
    if (k > p->ny) k = p->ny;

    FeatureWeights fws = FeatureWeights_create(p->nx);

    if (fws)
    {
        long indices[k];
        long freqindices[k];
        double distances[k];
        double freqs[k];

        // KNN_kNeighboursSkip(p, p, fws, y, k, indices, skipper); .OS.081011 ->
        if(!KNN_kNeighboursSkip(p, p, fws, y, k, indices, skipper))
            return(0);
        // .OS.081011 <-

        long ncategories = KNN_kIndicesToFrequenciesAndDistances(c, k, indices, distances, freqs, freqindices);

        forget(fws);

        if (FRIENDS(c->item[y], c->item[freqindices[KNN_max(freqs, ncategories)]]))
            return(1);
    }
    return(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// testing for criticalness                                                                //
/////////////////////////////////////////////////////////////////////////////////////////////

int KNN_prune_critical
(
    ///////////////////////////////
    // Parameters                //
    ///////////////////////////////

    Pattern p,      // source
                    //
    Categories c,   // source
                    //
    long y,         // source instance index
                    //
    long k          // k(!)
                    //
)

{
    if (y > p->ny) y = p->ny;   // safety belt
    if (k > p->ny) k = p->ny;

    FeatureWeights fws = FeatureWeights_create(p->nx);

    if (fws)
    {
        long indices[k];
        // long ncollected = KNN_kNeighboursSkip(p, p, fws, y, k, indices, 0); .OS.081011
        long ncollected = KNN_kNeighboursSkip(p, p, fws, y, k, indices, y);

        for (long ic = 0; ic < ncollected; ic++)
            if (!KNN_prune_superfluous(p, c, indices[ic], k, 0) || !KNN_prune_superfluous(p, c, indices[ic], k, y))
            {
                forget(fws);
                return(1);
            }
    }
    return(0);
}


/////////////////////////////////////////////////////////////////////////////////////////////
// testing for noisyness                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////////

int KNN_prune_noisy
(
    ///////////////////////////////
    // Parameters                //
    ///////////////////////////////

    Pattern p,      // source
                    //
    Categories c,   // source
                    //
    long y,         // source instance index
                    //
    long k          // k(!)
                    //
)

{
    if (y > p->ny) y = p->ny;   // safety belt
    if (k > p->ny) k = p->ny;

    FeatureWeights fws = FeatureWeights_create(p->nx);
    if (fws)
    {
        long indices[p->ny];    // the coverage is not bounded by k but by n
        // long reachability = KNN_kNeighboursSkip(p, p, fws, y, k, indices, 0); .OS.081011
        long reachability = KNN_kNeighboursSkip(p, p, fws, y, k, indices, y); 
        long coverage = KNN_prune_kCoverage(p, c, y, k, indices);

        forget(fws);
        if (!KNN_prune_superfluous(p, c, y, k, 0) && reachability > coverage)
            return(1);
    }
    return(0);
}

/* End of file KNN_prune.c */
