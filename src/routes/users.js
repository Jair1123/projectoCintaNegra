const router = require('express').Router();
const passport = require('passport');

//Models
const User = require('../models/User');



router.get('/users/signin', (req, res) => {
    res.render('Users/signin');
});

router.post('/users/signin', passport.authenticate('local', {
    successRedirect: '/contact/all',
    failureRedirect: '/users/signin',
    failureFlash: true
}))

router.get('/users/signup', (req, res) => {
    res.render('Users/signup');
});

router.post('/users/signup', async (req, res) => {
    const { name, email, password, confirm_password } = req.body
    const errors = [];

    if (name.length <= 0) {
        errors.push({ text: 'Inserta tu nombre' });
    }
    if (password != confirm_password) {
        errors.push({ text: 'Las contraseñas no son iguales' })
    }
    if (password.length < 4) {
        errors.push({ text: 'La contraseña debe ser mayor a 4 caracteres' });
    }
    if (errors.length > 0) {
        res.render('Users/signup', { errors, name, email, password, confirm_password });
    } else {
        const emailUser = await User.findOne({ email: email });
        if (emailUser) {
            req.flash('success_msg', 'El correo ya esta registrado');
            res.redirect('/users/signup');
        } else {
            const newUser = new User({ name, email, password });
            newUser.password = await newUser.encryptPassword(password);
            await newUser.save();
            req.flash('success_msg', 'Estas registrado')
            res.redirect('/users/signin');
        }
    }
});

router.get('/users/logout', (req, res) => {
    req.logOut();
    res.redirect('/');
})

module.exports = router;

