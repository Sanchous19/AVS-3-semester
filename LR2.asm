;������������ ������ �2
.MODEL SMALL
.STACK 100h

.DATA
	ten dw 10
	string db 15 dup(?)
	len dw ?
	dividendMessage db 'dividend: ', '$'
	divisorMessage db 'divisor: ', '$'
	quotientMessage db 'quotient: ', '$'
	remainderMessage db 'remainder: ', '$'
	inputErrorMessage db 'Input error, please enter the number again', 13, 10, '$'
	divideByZeroErrorMessage db 'Error, divide by zero', 13, 10, '$'
	endline db 13, 10, '$'
.CODE

output proc						; ��������� ������ ����� �� �������� AX � �������
	push ax
	push cx						; ���������� �������� �� ��������� � ����
	push dx
	push di
	xor cx,cx

convertToChar:						; ��������������� ���� � ������� � ������ � ����
	inc cx
	xor dx,dx
	div ten
	add dx,'0'					
	push dx
	test ax,ax
	jnz convertToChar
	
	lea di,string
putCharactersInString:				; ��������� �������� � ������
	pop dx
	mov [di],dl
	inc di						
	loop putCharactersInString
	mov byte ptr[di],'$'
	
	lea dx,string
	call printString					; ����������� ������ � �������
	call printEndline

	pop di
	pop dx
	pop cx						; ����������� �������� �� �����
	pop ax
	ret
output endp


input proc						; ��������� ������ ����� �� �������
	push bx
	push cx						; ���������� �������� �� ��������� � ����
	push dx
	xor bx,bx

inputCharacter:	
	mov ah,01h					
	inc cx
	int 21h
	cmp al,27
	je pressedEscape
	cmp al,8
	je pressedBackspace
	cmp al,13
	je theEndOfInput
	jmp addNewNumeral					; ������ ���� � ����������

addNewNumeral:							; ������� ������ � �����
	xor ah,ah
	xchg ax,bx
	cmp bl,'0'
	jb inputErrorLabel					; �������� �� ������������ �����
	cmp bl,'9'
	ja inputErrorLabel					
	sub bl,'0'							
	mul ten						
	jc inputErrorLabel
	add ax,bx
	jc inputErrorLabel
	xchg ax,bx
	jmp inputCharacter

pressedEscape:								; ��������� ������� �� ������� Escape
	call deleteLastSymbol
	loop pressedEscape
	xor bx,bx
	jmp inputCharacter

pressedBackspace:							; ��������� ������� �� ������� Backspace
	mov dl,' '
	call printSymbol
	call deleteLastSymbol
	dec cx
	cmp cx,0
	je inputCharacter
	xor dx,dx
	xchg ax,bx
	div ten
	xchg ax,bx
	dec cx
	jmp inputCharacter

inputErrorLabel:
	call printEndline
	call printInputErrorMessage						; ������������� ������ ����� � ���������
	xor bx,bx
	jmp inputCharacter

theEndOfInput:
	mov ax,bx
	pop dx
	pop cx							; ����������� �������� �� �����
	pop bx
	ret
input endp


deleteLastSymbol proc					; ��������� �������� ���������� ������� � �������
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


printString proc
	push ax
	mov ah,09h
	int 21h	
	pop ax
	ret
printString endp


printSymbol proc
	push ax
	mov ah,02h
	int 21h	
	pop ax
	ret
printSymbol endp


printDividendMessage proc						; ��������� ��������� �� ������� ����� "dividend"
	push dx
	lea dx,dividendMessage
	call printString
	pop dx
	ret
printDividendMessage endp


printDivisorMessage proc						; ��������� ��������� �� ������� ����� "divisor"
	push dx
	lea dx,divisorMessage
	call printString
	pop dx
	ret
printDivisorMessage endp


printQuotientMessage proc						; ��������� ��������� �� ������� ����� "quotient"
	push dx
	lea dx,quotientMessage
	call printString
	pop dx
	ret
printQuotientMessage endp


printRemainderMessage proc						; ��������� ��������� �� ������� ����� "remainder"
	push dx
	lea dx,remainderMessage
	call printString
	pop dx
	ret
printRemainderMessage endp


printInputErrorMessage proc					; ��������� ��������� ������ ��� ������� �� 0
	push dx
	lea dx,inputErrorMessage
	call printString
	pop dx
	ret
printInputErrorMessage endp


printDividedByZeroErrorMessage proc					; ��������� ��������� ������ ��� ������� �� 0
	push dx
	lea dx,divideByZeroErrorMessage
	call printString
	pop dx
	ret
printDividedByZeroErrorMessage endp


printEndline proc						; ��������� �������� ������� �� ������ ������
	push dx
	lea dx,endline
	call printString
	pop dx
	ret
printEndline endp


START:
    mov ax,@data
	mov ds,ax

	call input			
	call printDividendMessage
	call output					; ���� � ����� ��������

	mov bx,ax
	call input				
	call printDivisorMessage
	call output					; ���� � ����� ��������

	cmp ax,0
	jne divisorIsNotZero
	call printDividedByZeroErrorMessage
	jmp exit

divisorIsNotZero:
	xchg ax,bx
	xor dx,dx
	div bx

	call printQuotientMessage
	call output					; ����� ��������

	mov ax,dx
	call printRemainderMessage
	call output					; ����� �������

exit:
	mov ah,4ch
    int 21h
END START