#pragma	once

template < class Type > 
class LinkedList
{
public:

	struct Element
	{
		Type *data; // Pointer to the data held in the element.
		Element *next; // Pointer to the next element in the list.
		Element *prev; // Pointer to the previous element in the list.

		//---------------------------------------------------------------------
		// The element structure constructor.
		//---------------------------------------------------------------------
		Element( Type *element )
		{
			data = element;
			next = prev = NULL;
		}

		//---------------------------------------------------------------------
		// The element structure destructor.
		//---------------------------------------------------------------------
		~Element()
		{
			delete( data );

			if( next )
				next->prev = prev;
			if( prev )
				prev->next = next;
		}
	};

	LinkedList()
	{
		m_first = m_last = m_iterate = m_temp = NULL;
		m_totalElements = 0;
	}

	//-------------------------------------------------------------------------
	// The linked list class destructor.
	//-------------------------------------------------------------------------
	~LinkedList()
	{
		Empty();
	}

	//-------------------------------------------------------------------------
	// Adds the given element to the end of the linked list.
	//-------------------------------------------------------------------------
	Type *Add( Type *element )
	{
		if( element == NULL )
			return NULL;

		if( m_first == NULL )
		{
			m_first = new Element( element );
			m_last = m_first;
		}
		else
		{
			m_last->next = new Element( element );
			m_last->next->prev = m_last;
			m_last = m_last->next;
		}

		m_totalElements++;

		return m_last->data;
	}

	void AppendFirst()
	{
		if (m_first->next != NULL) 
		{
			m_first = m_first->next;
			return;
		} 
		else
		{
			return;
		}
	}
	//-------------------------------------------------------------------------
	// Inserts the given element into the linked list just before nextElement.
	//-------------------------------------------------------------------------
	Type *InsertBefore( Type *element, Element *nextElement )
	{
		m_temp = nextElement->prev;

		m_totalElements++;

		if( m_temp == NULL )
		{
			m_first = new Element( element );
			m_first->next = nextElement;
			nextElement->prev = m_first;

			return m_first->data;
		}
		else
		{
			m_temp->next = new Element( element );
			m_temp->next->prev = m_temp;
			m_temp->next->next = nextElement;
			nextElement->prev = m_temp->next;

			return m_temp->next->data;
		}
	}


	//-------------------------------------------------------------------------
	// Splits the Linked List in half [For potential multithreading]
	//-------------------------------------------------------------------------
	//LinkedList Split(LinkedList *secondList)
	LinkedList Split()
	{
		LinkedList* secondList;

		m_temp = m_first;
		int tempIndex = 0;

		for ( int i = 0; i < m_totalElements / 2; ++i )
		{
			m_temp = m_temp->next;
		}

		// Typename generates an int? Why?
		int tname = typename(this);

		// Typeid of temp returns that it's a struct linked list yada yada.
		// returns i = "struct LinkedList<class Zombie>::Element"
		string i = typeid(this).name();

		// This is a really hacked up way to determine if the linked list is of Zombie Template.
		// Can optimize later.
		if ( i == "class LinkedList<class Zombie> *" )
		{
			secondList = new LinkedList < Zombie > ();
		}

		//Debug to tell if the class is returning properly.. So far it is.
		i = typeid(secondList).name();

		while (m_temp)
		{
			break;
			//secondList->Add(*m_temp);
		}


		return *secondList;
		// This doesn't work right here.
		//string h = typeid(Zombie).name();
		//cout << h;


		// Check to see if it is this type of class? Howwww do I do this. 
		//if (m_temp == )
		//{
		//	LinkedList <Zombie> *newList;
		//}
		//secondList->Add(m_temp);

		/*
		LinkedList *tempList = new LinkedList();
		tempList = this;

		m_temp = m_first;
		

		for ( int i = 0; i < m_totalElements / 2; ++i )
		{
			m_temp = m_temp->next;
		}

		// Or could just make multithread only go to 1/2 this linked list (divide totalelements by 2);
		//Element m_split = m_temp;

		//secondList.Empty();

		// I think I have what m_temp is confused with what the elements of the list are. ***********
		while(m_temp = m_temp->next)
		{
			tempList->Add(m_temp);
		}

		if (m_totalElements > tempList->GetTotalElements() + 2)
			cout >> "Split needs to have first element be one next to be properly split \n";

		cout >> "Linked list has been split \n";
		*/
		//return tempList;
	}

	//-------------------------------------------------------------------------
	// Removes the given element from the linked list and destroys its data.
	//-------------------------------------------------------------------------
	void Remove( Type **element )
	{
		m_temp = m_first;
		while( m_temp != NULL )
		{
			if( m_temp->data == *element )
			{
				if( m_temp == m_first )
				{
					m_first = m_first->next;
					if( m_first )
						m_first->prev = NULL;
				}
				if( m_temp == m_last )
				{
					m_last = m_last->prev;
					if( m_last )
						m_last->next = NULL;
				}

				delete( m_temp );

				*element = NULL;

				m_totalElements--;

				return;
			}

			m_temp = m_temp->next;
		}
	}

	//-------------------------------------------------------------------------
	// Destroys all the elements in the linked list as well as their data.
	//-------------------------------------------------------------------------
	void Empty()
	{
		while( m_last != NULL )
		{
			m_temp = m_last;
			m_last = m_last->prev;
			delete( m_temp );
		}
		m_first = m_last = m_iterate = m_temp = NULL;
		m_totalElements = 0;
	}

	// Makes there only be one in the list. The first one.
	void SetToOne()
	{
		while( m_first != m_last)
		{
			m_temp = m_last;
			m_last = m_last->prev;
		}

		m_totalElements = 1;
			
	}

	//-------------------------------------------------------------------------
	// Removes all the elements and clears their data pointers.
	// Warning: This function does not destroy the data held be each element.
	//-------------------------------------------------------------------------
	void ClearPointers()
	{
		while( m_last != NULL )
		{
			m_temp = m_last;
			m_temp->data = NULL;
			m_last = m_last->prev;
			delete( m_temp );
		}
		m_first = m_last = m_iterate = m_temp = NULL;
		m_totalElements = 0;
	}

	//-------------------------------------------------------------------------
	// Removes the given element and clears its data pointer.
	// Warning: This function does not destroy the data held by the element.
	//-------------------------------------------------------------------------
	void ClearPointer( Type **element )
	{
		m_temp = m_first;
		while( m_temp != NULL )
		{
			if( m_temp->data == *element )
			{
				if( m_temp == m_first )
				{
					m_first = m_first->next;
					if( m_first )
						m_first->prev = NULL;
				}
				if( m_temp == m_last )
				{
					m_last = m_last->prev;
					if( m_last )
						m_last->next = NULL;
				}

				m_temp->data = NULL;

				delete( m_temp );

				*element = NULL;

				m_totalElements--;

				return;
			}

			m_temp = m_temp->next;
		}
	}

	//-------------------------------------------------------------------------
	// Iterates through the elements in the linked list.
	//-------------------------------------------------------------------------
	Type *Iterate( bool restart = false )
	{
		if( restart )
			m_iterate = NULL;
		else
		{
			if( m_iterate == NULL )
				m_iterate = m_first;
			else
				m_iterate = m_iterate->next;
		}

		if( m_iterate == NULL )
			return NULL;
		else
			return m_iterate->data;
	}

	//-------------------------------------------------------------------------
	// Returns the currently iterated element in the linked list.
	//-------------------------------------------------------------------------
	Type *GetCurrent()
	{
		if( m_iterate )
			return m_iterate->data;
		else
			return NULL;
	}

	//-------------------------------------------------------------------------
	// Returns the first element in the linked list.
	//-------------------------------------------------------------------------
	Type *GetFirst()
	{
		if( m_first )
			return m_first->data;
		else
			return NULL;
	}

	//-------------------------------------------------------------------------
	// Returns the last element in the linked list.
	//-------------------------------------------------------------------------


		Type *GetThis( Type *element)
	{
		m_temp = m_first;
		while( m_temp != NULL )
		{
			if( m_temp->data == element )
			{
					return m_temp->data;
			}

			if( m_temp->next == NULL)
			{
				return NULL;
			}
			m_temp = m_temp->next;
		}

		return NULL;
	}

	//-------------------------------------------------------------------------
	// Returns the next element in the linked list from the given element.
	//-------------------------------------------------------------------------
	Type *GetNext( Type *element )
	{
		m_temp = m_first;
		while( m_temp != NULL )
		{


				if( m_temp->data == element )
				{
					try
					{
						if( m_temp->next == NULL )
							return NULL;
						else
							return m_temp->next->data;
					}
					catch(exception e)
					{
						return NULL;
					}

				}


				m_temp = m_temp->next;
			

		}

		return NULL;
	}

	//-------------------------------------------------------------------------
	// Returns a random element from the linked list.
	//-------------------------------------------------------------------------
	Type *GetRandom()
	{
		if( m_totalElements == 0 )
			return NULL;
		else if( m_totalElements == 1 )
			return m_first->data;

		unsigned long element = rand() % m_totalElements;

		m_temp = m_first;
		for( unsigned long e = 0; e < element; e++ )
			m_temp = m_temp->next;

		return m_temp->data;
	}

	//-------------------------------------------------------------------------
	// Returns the complete element (including its next and previous pointers).
	//-------------------------------------------------------------------------
	Element *GetCompleteElement( Type *element )
	{
		m_temp = m_first;
		while( m_temp != NULL )
		{
			if( m_temp->data == element )
				return m_temp;

			m_temp = m_temp->next;
		}

		return NULL;
	}

	//-------------------------------------------------------------------------
	// Returns the total number of elements in the linked list.
	//-------------------------------------------------------------------------
	unsigned long GetTotalElements()
	{
		return m_totalElements;
	}

private:

	Element *m_first; // First element in the linked list.
	Element *m_last; // Last element in the linked list.
	Element *m_iterate; // Used for iterating the linked list.
	Element *m_temp; // Used for temporary storage in various operations.

	unsigned long m_totalElements; // Total number of elements in the linked list.

};

