const router = require('express').Router();
const Contact = require('../models/contact')

const { isAuthenticated } = require('../helpers/auth')

router.get('/', (req, res) => {
    res.render('index');
})

router.get('/dashboard',isAuthenticated, (req, res) => {
    res.render('esp');
})

router.get('/price', (req, res) => {
    res.render('price')
})

router.get('/contact', (req, res) => {
    res.render('contact')
})



router.post('/contact/add', async (req, res) => {
    // console.log(req.body);
    const { nombre, correo, mensaje } = req.body;
    const errors = [];
    if (!nombre) {
        errors.push({ text: "Please Write a name" });
    }
    if (!correo) {
        errors.push({ text: "Please Write your mail" });
    }
    if (!mensaje) {
        errors.push({ text: "Please Write a message" });
    }
    if (errors.length > 0) {
        res.render('contact', {
            errors,
            nombre,
            correo,
            mensaje
        })
    } else {
        //Vamos a almacenar estos datos en la base de datos su archivo estara en models llamado contact.js
        const newContact = new Contact({ nombre, correo, mensaje });
        await newContact.save();
        req.flash('success_msg','mensaje enviado')
        res.redirect('/contact');
    }

})

router.get('/contact/all',isAuthenticated, async (req, res) => {
    const allcontact = await Contact.find().sort({ date: 'desc' });
    res.render('allcontact', { allcontact });
})

router.delete('/contact/all/delete/:id',isAuthenticated, async (req, res) => {
    await Contact.findByIdAndDelete(req.params.id);
    req.flash('success_msg','mensaje eliminado')
    res.redirect('/contact/all');
})


module.exports = router;
