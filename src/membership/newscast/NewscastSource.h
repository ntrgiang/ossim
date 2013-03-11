/*
 * NewscastSource.h
 *
 *      Author: Thorsten Jacobi
 */

// @author Thorsten Jacobi
// @brief the implementation of the newscast source, inherits NewscastBase
// @ingroup membership
// @ingroup newscast

#include "NewscastBase.h"

#ifndef NEWSCASTSOURCE_H_
#define NEWSCASTSOURCE_H_

class NewscastSource : public NewscastBase {
public:
    NewscastSource();
    virtual ~NewscastSource();
};

#endif /* NEWSCASTSOURCE_H_ */
