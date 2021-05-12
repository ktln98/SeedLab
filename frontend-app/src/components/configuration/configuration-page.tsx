import Button from '@material-ui/core/Button';
import TextField from '@material-ui/core/TextField';
import React, { useState } from 'react';
import './configuration-page.scss'

export default function Configuration(props){
    const [plantName, setPlantName] = useState('');
    const [selectedDate, setSelectedDate] = useState(new Date().toLocaleString());
    const [extraInfo, setExtraInfo] = useState('');
    
    function configHandler(event){
        event.preventDefault();
        props.configDataProvider({
            plantName: plantName,
            plantDate: selectedDate,
            extraInfo: extraInfo
            }
        )
    }

    return(
        <div className="configuration">
            <div className="configuration-container">
                <div className="title-config">
                    <label>Bine ați venit pe pagina de configurare</label>
                </div>
                <div className="form-div">
                    <form className="form-container"  onSubmit={configHandler}>
                        <div className="item">
                            <label>Ce ați plantat?</label>
                            <TextField id="outlined-basic" variant="outlined" value={plantName} onChange={e => setPlantName(e.target.value)} />
                        </div>
                        <div className="item">
                            <TextField id="date" label="Data plantării" type="date" defaultValue={selectedDate} InputLabelProps={{shrink: true}}  onChange={e => setSelectedDate(e.target.value)}/>
                        </div>
                        <div className="item">
                            <label>Doriți sa adăugați informații suplimentare?</label>
                            <TextField id="outlined-multiline-static" multiline rows={4} variant="outlined" value={extraInfo} onChange={e => setExtraInfo(e.target.value)} />
                        </div>
                        <div className="btn-config">
                            <Button color="primary" variant="contained" type="submit">Configureaza</Button>
                        </div>
                    </form>
                </div>
                <div className="instructions">
                    <label>Pagina de configurare are rol de notebook. Aici, utilizatorul are posibilitatea să își noteze câteva detalii legate de plantația sa cum ar fi:
                        tipul de plantă, data plantării, cât și informații adiționale.
                    </label>
                </div>
            </div>
        </div>
    )
}