public class PilaEstatica {
    private int maxSize;
    private int[] stackArray;
    private int top;

    public PilaEstatica(int size) {
        this.maxSize = size;
        this.stackArray = new int[maxSize];
        this.top = -1;
    }

    public void initStack() {
        this.top = -1;
    }

    public boolean isFull() {
        return top == maxSize - 1;
    }

    public boolean isEmpty() {
        return top == -1;
    }

    public boolean push(int value) {
        if (isFull()) {
            System.out.println("Stack is full. Cannot push " + value);
            return false;
        } else {
            stackArray[++top] = value;
        }
        return true;
    }

    public int pop() {
        if (isEmpty()) {
            System.out.println("Stack is empty. Cannot pop.");
            return -1; // or throw an exception
        } else {
            return stackArray[top--];
        }
    }

    public void displayStack() {
        if (isEmpty()) {
            System.out.println("Stack is empty.");
        } else {
            System.out.print("Stack elements: ");
            for (int i = top; i >= 0; i--) {
                System.out.print(stackArray[i] + " ");
            }
            System.out.println();
        }
    }

    public int returnTop() {
        if (isEmpty()) {
            System.out.println("Stack is empty. No top element.");
            return -1; // or throw an exception
        } else {
            return stackArray[top];
        }
    }

    public void resetStack() {
        initStack();
        System.out.println("Stack has been reset.");
    }

    public static void main(String[] args) {
        PilaEstatica stack = new PilaEstatica(5);
        java.util.Scanner scanner = new java.util.Scanner(System.in);
        int option, value;

        do {
            System.out.println("1. Push");
            System.out.println("2. Pop");
            System.out.println("3. Display Stack");
            System.out.println("4. Return Top Element");
            System.out.println("5. Reset Stack");
            System.out.println("6. Exit");
            System.out.print("Enter your option: ");
            option = scanner.nextInt();

            switch (option) {
                case 1:
                    System.out.print("Enter value to push: ");
                    value = scanner.nextInt();
                    stack.push(value);
                    break;
                case 2:
                    int poppedValue = stack.pop();
                    if (poppedValue != -1) {
                        System.out.println("Popped value: " + poppedValue);
                    }
                    break;
                case 3:
                    stack.displayStack();
                    break;
                case 4:
                    int topValue = stack.returnTop();
                    if (topValue != -1) {
                        System.out.println("Top element: " + topValue);
                    }
                    break;
                case 5:
                    stack.resetStack();
                    break;
                case 6:
                    System.out.println("Exiting...");
                    break;
                default:
                    System.out.println("Invalid option. Please try again.");
            }
        } while (option != 6);

        scanner.close();
    }
}