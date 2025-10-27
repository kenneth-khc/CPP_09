/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 21:39:14 by kecheong          #+#    #+#             */
/*   Updated: 2025/10/19 21:39:15 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <set>
#include <ctime>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <utility>
#include <iostream>
#include <algorithm>

template <template <typename T, typename = std::allocator<T> > class Container>
class PmergeMe
{
	/* internal Int wrapper */
	struct Int;

public:
	typedef std::size_t								size_type;

	/* iterator of a std::set<int> */
	typedef typename std::set<int>::iterator		SetIter;

	/* numbers sequentially within a container, whether it is
	   a std::vector or std::deque. */
	typedef Container<Int>							Sequence;
	typedef typename Sequence::iterator				Iterator;
	typedef typename Sequence::const_iterator		ConstIterator;
	typedef typename Sequence::reverse_iterator		ReverseIterator;

	/* a pair of sequences, used to compare each rightmost number
	   to sort the group as a whole. */
	typedef std::pair<Sequence, Sequence>			SequencePair;

	/* a sequence containing many subsequences, representing the
	   partially sorted main chain to be inserted into. */
	typedef Container<Sequence>						MainChain;
	typedef typename MainChain::iterator			MainChainIter;

	/* a sequence with a bound attached to it, which is its
	   corresponding leader's index within the main chain.
	   this is an element within the pend, to be inserted into
	   the main chain  */
	typedef std::pair<Sequence, size_type>			BoundedSequence;
	/* the pend itself, containing loser sequences bounded by the
	   winners in the main chain. */
	typedef Container<BoundedSequence>				Pend;
	typedef typename Pend::iterator					PendIterator;
	typedef typename Pend::reverse_iterator			PendRevIterator;

	/* OCF */
public:
	PmergeMe() {};
	PmergeMe(int count, char** args)
	{
		parse(count, args);
	}
	~PmergeMe() {};
private:
	PmergeMe(const PmergeMe&) {};
	PmergeMe	operator=(const PmergeMe&) { return PmergeMe(); };

public:
	void	parse(std::size_t count, char** args)
	{
		// stores all the numbers so far, so that we can easily
		// check for duplicates
		std::set<int>				parsedNumbers;
		std::pair<SetIter, bool>	result;

		Int::amount = 0;
		for (std::size_t i = 0; i < count; ++i)
		{
			int	num = validatePositiveInteger(args[i]);
			result = parsedNumbers.insert(num);
			if (result.second == false)
			{
				std::cerr << "Error: duplicate number `" << num
						  << "` found, expected unique numbers" << std::endl;
				std::exit(1);
			}
			numbers.push_back(num);
		}
	}

	Container<int>	mergeInsertionSort() const
	{
		Sequence	unsorted = numbers;
		size_type	elementSize = mergePairs(unsorted);
		Sequence	sorted = performInsertions(unsorted, elementSize);

		return unbox(sorted);
	}

	Container<int>
	benchmark(Container<int> (PmergeMe<Container>::*sortingFunction)() const)
	{
		struct timespec	execStart, execEnd;

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &execStart);

		Container<int> sorted = (this->*sortingFunction)();

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &execEnd);

		const double	BILLION = 1000000000.;

		double startNanosec = execStart.tv_sec * BILLION + execStart.tv_nsec;
		double endNanosec = execEnd.tv_sec * BILLION + execEnd.tv_nsec;
		timeSpentExecuting = endNanosec - startNanosec;

		return sorted;
	}

	void	displayTimeTaken(const std::string& label) const
	{
		const double	THOUSAND = 1000.;
		const double	MILLION = 1000000.;
		const double	BILLION = 1000000000.;

		std::size_t	size = numbers.size();
		const char*	message = size == 1 ? "element" : "elements";
		std::cout << "Time to process a range of " << size << ' ' << message
				  << " with " << label << " : "
				  << std::fixed << timeSpentExecuting / THOUSAND << " μs" << " / "
				  << std::fixed << timeSpentExecuting / MILLION << " ms" << " / "
				  << std::fixed << timeSpentExecuting / BILLION << " s" << '\n';

	#ifdef TESTS
		logAndResetComparisons();
	#endif
	}


private:
	Sequence		numbers;
	double			timeSpentExecuting;

		/* we keep forming pairs to compare against each other, until we can't form
	   even a single pair.

	   example: [] being the sequence, {} being a pair, elements separated by |
	   [5, 3, 2, 4, 6, 1]
	   1st iteration: {5|3} {2|4} {1|6} => {3|5} {2|4} {1|6}
	   2nd iteration: {3,5 | 2,4}  1 6  => {2,4 | 3,5}  1 6
	   3rd iteration: can't form any pair as we would need at least 8 numbers.

	   when there are leftover numbers that can't participate in comparing, we
	   leave them be and simply add them to the back after the comparisons. */
	static size_type	mergePairs(Sequence& unsorted)
	{
		Sequence	s = unsorted;
		size_type	elementSize = 1;

		while (elementSize <= s.size() / 2)
		{
			size_type	elementCount = countElements(s.size(), elementSize);
			Sequence	leftovers = storeLeftovers(s, elementCount, elementSize);
			Container<SequencePair>	pairs = createPairs(s, elementCount, elementSize);
			sortPairs(pairs);
			s = flatten(pairs);
			s.insert(s.end(), leftovers.begin(), leftovers.end());
			elementSize *= 2;
		}
		unsorted = s;
		return elementSize / 2;
	}

	/* counts the maximum possible number of elements we can form given an
	   amount of numbers and the expected size of each element.
	   if it turns out to be an odd number, we round it down to the next
	   lowest even number as elements have to be in pairs and we should
	   disregard lone elements. */
	static size_type	countElements(size_type numCount, size_type elementSize)
	{
		size_type	elementCount = numCount / elementSize;
		if (elementCount % 2 != 0)
		{
			return elementCount - 1;
		}
		return elementCount;
	}

	/** calculate the leftover elements that will not be participating in
	    comparisons and store them temporarily */
	static Sequence	storeLeftovers(const Sequence& sequence,
								   size_type eleCount,
								   size_type eleSize)
	{
		Sequence	leftovers;
		size_type	leftoverCount = sequence.size() %
									(eleCount * eleSize);
		if (leftoverCount)
		{
			ConstIterator	iter = sequence.end() - leftoverCount;
			leftovers.insert(leftovers.end(),
			                 iter,
							 sequence.end());
		}
		return leftovers;
	}

	/** given a sequence of numbers, create elementCount elements
	    with each being elementSize size and wrap each two in a
	    std::pair, returning a container of the pairs */
	static Container<SequencePair>	createPairs(const Sequence& sequence,
												size_type elementCount,
												size_type elementSize)
	{
		Container<SequencePair>	pairs;
		ConstIterator			iter = sequence.begin();
		size_type				maxPairs = elementCount / 2;

		for (size_type pairsCreated = 0;
			 pairsCreated < maxPairs;
			 ++pairsCreated)
		{
			Sequence	first(iter, iter + elementSize);
			iter += elementSize;
			Sequence	second(iter, iter + elementSize);
			iter += elementSize;
			pairs.push_back(std::make_pair(first, second));
		}
		return pairs;
	}
	
	/** goes through each pair, and sorts each element in place
	    within by looking at the rightmost number */
	static void	sortPairs(Container<SequencePair>& pairs)
	{
		typename Container<SequencePair>::iterator	iter = pairs.begin();
		typename Container<SequencePair>::iterator	end = pairs.end();
		for (; iter != end; ++iter)
		{
			Int&	lhs = iter->first.back();
			Int&	rhs = iter->second.back();
			if (rhs < lhs)
			{
				std::swap(iter->first, iter->second);
			}
		}
	}

	/** flattens a container of pairs into a single sequence */
	static Sequence flatten(const Container<SequencePair>& pairs)
	{
		Sequence	container;
		typename Container<SequencePair>::const_iterator	iter = pairs.begin();
		typename Container<SequencePair>::const_iterator	end = pairs.end();
		for (; iter != end; ++iter)
		{
			container.insert(container.end(), iter->first.begin(), iter->first.end());
			container.insert(container.end(), iter->second.begin(), iter->second.end());
		}
		return container;
	}

	/** flattens a container of sequences into a single sequence */
	static Sequence flatten(const Container<Sequence>& sequences)
	{
		Sequence	container;
		typename Container<Sequence>::const_iterator	iter = sequences.begin();
		typename Container<Sequence>::const_iterator	end = sequences.end();
		for (; iter != end; ++iter)
		{
			container.insert(container.end(), iter->begin(), iter->end());
		}
		return container;
	}

	/** starts performing insertions by initializing the main chain and pend elements.
	    the size of each element is initially determined by the maximum element size
	    we can form during merging. then, it halves itself until we hit 0, at which
	    point everything would be sorted and we are done. */
	static Sequence	performInsertions(const Sequence& copy, size_type elementSize)
	{
		Sequence	s = copy;

		while (elementSize >= 1)
		{
			size_type	elementCount =  s.size() / elementSize;
			Sequence	leftovers = storeLeftovers(s,
												   elementCount,
												   elementSize);
			size_type					created = 0;
			Container<Sequence>			main;
			Container<BoundedSequence>	pend;
			typename Sequence::iterator	iter = s.begin();
			for (bool a = false;; a = !a)
			{
				Sequence	e(iter, iter + elementSize);
				if (a || created == 0)
				{
					main.push_back(e);
				}
				else
				{
					pend.push_back(std::make_pair(e, main.size()));
				}
				iter += elementSize;
				if (++created >= elementCount)
				{
					break;
				}
			}
			if (!pend.empty())
			{
				insertPendElementsIntoMainChain(main, pend);
			}
			s = flatten(main);
			s.insert(s.end(), leftovers.begin(), leftovers.end());
			elementSize /= 2;
		}
		return s;
	}

	/** as long as the pend has elements remaining, we keep trying to insert
	    them into the main chain, following the Jacobsthal sequence. */
	static void
	insertPendElementsIntoMainChain(Container<Sequence>& main,
									Container<BoundedSequence>& pend)
	{
		unsigned long	term = 3;
		size_type		insertedCount = 0;

		while (!pend.empty())
		{
			unsigned long	jacobsthalNumber = jacobsthal(term);
			PendRevIterator	toPend = pend.rbegin();
			PendRevIterator	iter = pend.rbegin();
			PendRevIterator	end = pend.rend();
			// we start from the end of the pend and find the matching
			// jacobsthal number for the bound index
			for (;
				 iter != end && toPend->second > jacobsthalNumber;
				 ++iter)
			{
				if (iter->second <= jacobsthalNumber)
				{
					toPend = iter;
				}
			}
			while (toPend != pend.rend())
			{
				const Sequence&	element = toPend->first;
				size_type		boundIndex = toPend->second;
				MainChainIter	searchEnd = main.begin() + boundIndex + insertedCount;
				MainChainIter	insertPos =
					std::lower_bound<
					typename Container<Sequence>::iterator,
					Sequence,
					/* the comparator type has to be written explicitly or it
					   fails to deduce it what the helly */
					bool(*)(const Sequence&, const Sequence&)
					>
					(main.begin(),
					searchEnd,
					element,
					compareLessThan);

				main.insert(insertPos, element);
				++insertedCount;

				/* .erase() only accepts forward iterators so we have to
				   get the reverse's underlying base iterator then decrement
				   it to get the correct element to erase. erasing it
				   returns the next new iterator and old ones are invalidated.
				   then convert the new forward iterator into a reverse iterator
				   for us to continue on. */
				PendIterator	newIter = pend.erase(--(toPend.base()));
				toPend = typename Container<BoundedSequence>::reverse_iterator(newIter);
			}
			++term;
		}
	}

	/** https://en.wikipedia.org/wiki/Jacobsthal_number/
	    tldr: the sequence starts at 0 and 1, then each following
	    number is the previous number plus twice the previous previous number
	    jacobsthal(2) = (0 * 2) + 1 = 1
	    jacobsthal(3) = (1 * 2) + 1 = 3
	    jacobsthal(4) = (1 * 2) + 3 = 5
	    jacobsthal(5) = (3 * 2) + 5 = 11
	    jacobsthal(6) = (5 * 2) + 11 = 21
	    ... */
	static unsigned int	jacobsthal(unsigned int n)
	{
		if (n == 0)
		{
			return 0;
		}
		else if (n == 1)
		{
			return 1;
		}
		return jacobsthal(n - 1) + jacobsthal(n - 2) * 2;
	}

	/** only accept positive integers within the range of ints,
	    otherwise we kill the program */
	static int	validatePositiveInteger(const std::string& arg)
	{
		char*		parseEnd = NULL;
		const char*	end = arg.c_str() + arg.size();
		long		num = std::strtol(arg.c_str(), &parseEnd, 0);
		if (parseEnd != end)
		{
			std::cerr << "Error: unexpected `" << arg << "`, expected a number\n";
			std::exit(1);
		}
		if (num < 0)
		{
			std::cerr << "Error: unexpected negative number `" << num << "`\n";
			std::exit(1);
		}
		if (num > std::numeric_limits<int>::max())
		{
			std::cerr << "Error: unexpected number `" << num
					  << "`, expected integer between 0-"
					  << std::numeric_limits<int>::max() << '\n';
			std::exit(1);
		}
		return num;
	}

	static void	logAndResetComparisons()
	{
	    int sum = 0;
	    for (int k = 1; k <= Int::amount; ++k)
	    {
	        double value = (3.0 / 4.0) * k;
	        sum += static_cast<int>(ceil(log2(value)));
	    }
	    const char* BLUE = "\x1B[0;34m";
	    const char* RESET = "\x1B[0m";
	    std::cout << BLUE << "For " << Int::amount << " numbers, expected at most "
				  << sum << " comparisons, "
				  << "total comparisons: " << Int::comparisons << RESET << '\n';
		Int::comparisons = 0;
	}

	/* a wrapper around an int in order to overload the comparison
	   operators to keep track of comparison counts */
	struct Int
	{
		/* how many Ints are directly constructed? this determines
		   the worst case of comparisons */
		static int	amount;

		static int	comparisons;

		int	value;
		Int(int value): value(value) { ++amount; };
		Int(const Int& other): value(other.value) {};
		~Int() {};

		bool	operator<(const Int& rhs) const
		{
			++comparisons;
			// std::cout << value << " < " << rhs.value << '\n';
			return this->value < rhs.value;
		}
		bool	operator>(const Int& rhs) const
		{
			++comparisons;
			// std::cout << value << " > " << rhs.value << '\n';
			return this->value > rhs.value;
		}
		bool	operator==(const Int& rhs) const
		{
			++comparisons;
			// std::cout << value << " == " << rhs.value << '\n';
			return this->value == rhs.value;
		}
		Int		operator=(const Int& other)
		{
			this->value = other.value;
			return *this;
		}

		operator int () const
		{
			return value;
		}
	};

	template <typename Comparable>
	static bool	compareLessThan(const Comparable& lhs, const Comparable& rhs)
	{
		return lhs.back() < rhs.back();
	}

	Container<int>	unbox(const Sequence& boxed) const
	{
		Container<int>	unboxed;
		for (ConstIterator iter = boxed.begin();
			 iter != boxed.end();
			 ++iter)
		{
			unboxed.push_back(*iter);
		}
		return unboxed;
	}

#ifdef TESTS
public:
	Container<int>	bubbleSort() const
	{
		Sequence	sequence = numbers;
		int 		n = sequence.size() - 1;

		for (int i = 0; i < n; ++i)
		{
			bool swapped = false;
			for (int j = 0; j < n - i; ++j)
			{
				if (sequence[j] > sequence[j+1])
				{
					std::swap(sequence[j], sequence[j+1]);
					swapped = true;
				}
			}
			if (!swapped) break;
		}
		return unbox(sequence);
	}

	Container<int>	insertionSort() const
	{
		Sequence	sequence = numbers;
		int			n = sequence.size();

		for (int i = 0; i < n; ++i)
		{
			Int key = sequence[i];
			int	j = i - 1;

			while (j >= 0 && sequence[j].operator>(key))
			{
				sequence[j+1]= sequence[j];
				j = j - 1;
			}
			sequence[j + 1] = key;
		}
		return unbox(sequence);
	}
#endif
};

template <template <typename T, typename A> class Container>
int	PmergeMe<Container>::Int::comparisons = 0;

template <template <typename T, typename A> class Container>
int	PmergeMe<Container>::Int::amount = 0;
