/*let PageHandler = {
    id: "123456",
    init: function() {
        document.addEventListener("click", (event) => {
            this.doSomething(event.type);
        });
    },
    doSomething: function(type) {
        console.log("Handling " + type + " for " + this.id);
    }
};

PageHandler.init();*/




/*let person1 = 
{
    getGreeting() 
    {
        return "Hello";
    }
};

let friend = 
{
    getGreeting() 
    {
        return  super.getGreeting.call(this) + ", hi!";
    }
};

Object.setPrototypeOf(friend, person1);
let relative = Object.create(friend);

console.log(person.getGreeting());
console.log(friend.getGreeting());
console.log(relative.getGreeting());

let p ={
    a : 1,
    b: 2
}
let {a : c, b : d} = p;

console.log(c);
console.log(d);



let person = {
    firstName: 'moishe',
    lastName: 'aharoni',
    location: {
        start: {
            X: 1,
            Y: 2
        },
        end: {
            X: 1,
            Y: 3
        }
    }
};

// let {firstName, lastName, location: {start, end}} = person;
let {firstName, lastName, location: {start, end}} = person;

console.log(firstName);
console.log(lastName);
console.log(start);
console.log(end);


class LinkedList {
        constructor(){
            this.size = 0;
        }    
        push(data){}
    }

    console.log(typeof LinkedList);     
    console.log(typeof LinkedList.prototype.push);
    */
function f()
{
    let a;
    function foo()
    {
        a = 7;
    }

    console.log(a);
}

f();