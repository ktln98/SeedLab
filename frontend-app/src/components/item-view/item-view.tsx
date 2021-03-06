import React from 'react';
import './item-view.scss'

export default function ItemView(props){
    return(
        <div className="item-view">
            <div className="name">
                <label>
                     {props.name} 
                </label>
            </div>
            <div className="value-container">
                <div className="value-unit">
                    <label className="value">{props.value}</label>
                    <label className="unit">{props.unit}</label> 
                </div>
                <img src={props.icon}  className="icon-design" alt="Not found"/>
            </div>
        </div>
    )
}