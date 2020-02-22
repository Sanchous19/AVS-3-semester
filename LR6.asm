; Лабораторная №6(Вариант 8)
.MODEL SMALL
.STACK 100h

.DATA
	vocabulary db 500 dup(?)
	inputWord db 30 dup(?)
	len dw 0
	handle dw 1
	fileName db 'words.txt', 0
	fileErrorMessage db 'Error with file', 13, 10, '$'
	endline db 13, 10, '$'
.CODE

readVocabularyFromFile proc						; Процедура чтения словаря из файла
	push ax
	push bx
	push cx
	push dx

	mov ah,3dH
	lea dx,fileName
	xor al,al
	int 21h										; Открытие файла
	jnc fileIsOpen
	call errorWithFile
fileIsOpen:
	mov [handle],ax
	mov bx,ax
	mov ah,3fH
	lea dx,vocabulary
	mov cx,500
	int 21h										; Чтение файла
	jnc fileIsRead
	call errorWithFile
fileIsRead:
	lea di,vocabulary
	add di,ax
	mov byte ptr[di],13
	inc di
	mov byte ptr[di],10
	add ax,2
	mov len,ax

	mov ah,3eH									; Закрытие файла
	mov bx,[handle]
	int 21h
	jnc fileIsClose
	call errorWithFile
fileIsClose:

	pop dx
	pop cx
	pop bx
	pop ax
	ret
readVocabularyFromFile endp


errorWithFile proc								; Процедура, обрабатывающая ошибки с файлами
	lea dx,fileErrorMessage
	call printString
	mov ah,4ch
    int 21h
errorWithFile endp


input proc
	push ax
	push bx
	push cx											; Сохранение значений из регистров в стек
	push dx
	push di
	push si
	lea di,inputWord
	xor bx,bx
	xor cx,cx

inputCharacter:										; Проверка на нажатие клавиши
	mov ah,01h
	int 21h
	cmp al,13
	je theEndOfInput
	cmp al,32
	je pressedSpace
	mov [di],al
	inc di
	inc cx
	jmp inputCharacter

pressedSpace:										; обработка нажатия на клавишу Space
	call findWordInVocabulary
	xor cx,cx
	lea di,inputWord
	jmp inputCharacter

theEndOfInput:
	pop si
	pop di
	pop dx
	pop cx											; Возвращение значений из стека
	pop bx
	pop ax
	ret
input endp


findWordInVocabulary proc							; Процедура нахождения слова в словаре
	push ax
	push bx
	push cx
	push di
	push si
	mov byte ptr[di],9
	inc cx
	lea di,vocabulary

compareWords:
	push cx
	lea si,inputWord
	repe cmpsb										; Проверяем слова на равенство
	je changeWord
	mov al,10
	mov cx,50
	repne scasb										; Ищем начало нового слова 
	mov ax,di
	lea bx,vocabulary
	sub ax,bx
	pop cx
	cmp ax,len										; Проверяем достигли ли мы конца словаря
	je theEndOfFindWord
	jmp compareWords

changeWord:
	pop cx
	call deleteWordFromConsole						; Удаляем слово из консоли
printWordInConsole:
	mov dl,[di]
	call printSymbol								; Выводим новое слово на консоль
	inc di
	cmp byte ptr[di],13
	jne printWordInConsole

theEndOfWord:
	jne theEndOfFindWord
	mov dl,' '
	call printSymbol								; Выводим пробел

theEndOfFindWord:
	pop si
	pop di
	pop cx
	pop bx
	pop ax
	ret
findWordInVocabulary endp


deleteWordFromConsole proc							; Процедура удаления последнего символа в консоли
	push cx
	push dx
	je delete
	dec cx
delete:
	call deleteLastSymbol
	loop delete
	pop dx
	pop cx
	ret
deleteWordFromConsole endp


deleteLastSymbol proc								; Процедура удаления последнего символа в консоли
	push dx
	mov dl,8
	call printSymbol
	mov dl,32
	call printSymbol
	mov dl,8
	call printSymbol
	pop dx
	ret
deleteLastSymbol endp


printSymbol proc									; Процедура выводящая символ
	push ax
	mov ah,02h
	int 21h	
	pop ax
	ret
printSymbol endp


printString proc									; Процедура выводящая строку
	push ax
	mov ah,09h
	int 21h	
	pop ax
	ret
printString endp


printEndline proc									; Процедура переноса каретки на другую строку
	push dx
	lea dx,endline
	call printString
	pop dx
	ret
printEndline endp


START:
    mov ax,@data
	mov ds,ax
	mov es,ax
	
	call readVocabularyFromFile
	call input

	mov ah,4ch
    int 21h
END START