PokemonsBattles:Pokemon.o MainSecond.o LinkedList.o MaxHeap.o BattleByCategory.o
	gcc -g LinkedList.o MainSecond.o Pokemon.o MaxHeap.o BattleByCategory.o -o PokemonsBattles
Pokemon.o: Pokemon.c Pokemon.h Defs.h
	gcc -c -g Pokemon.c
MainSecond.o: MainSecond.c Pokemon.h BattleByCategory.h Defs.h
	gcc -c -g MainSecond.c
MaxHeap.o: MaxHeap.c MaxHeap.h	
	gcc -c -g MaxHeap.c	
LinkedList.o: LinkedList.c LinkedList.h
	gcc -c -g LinkedList.c
BattleByCategory.o: BattleByCategory.c BattleByCategory.h LinkedList.h MaxHeap.h Defs.h
	gcc -c -g BattleByCategory.c		
clean:
	rm -f *.o PokemonsBattles
