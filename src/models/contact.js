const mongoose =  require('mongoose');
//este archivo se usara para crear un modelo llamado en index.js
const {Schema} = mongoose; 

const contactSchema =  new Schema({
    nombre: {type: String,required:true},
    correo: {type: String,required:true},
    mensaje: {type: String,required:true},
    date: {type:Date,default:Date.now}
});

module.exports = mongoose.model('Contact',contactSchema);